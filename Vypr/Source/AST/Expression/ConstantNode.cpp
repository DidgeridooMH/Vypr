#include "Vypr/AST/Expression/ConstantNode.hpp"

#include <codecvt>
#include <locale>

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"
#include "Vypr/AST/Type/RealType.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Lexer/CLangToken.hpp"
#include "Vypr/Util/Overload.hpp"

namespace Vypr
{
  ConstantNode::ConstantNode(std::unique_ptr<StorageType> &&type,
                             ConstantValue value, size_t column, size_t line)
      : ExpressionNode(std::move(type), column, line), m_value(value)
  {
  }

  std::wstring ConstantNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"Constant(";
    std::visit(
        overloaded{
            [&](auto arg) { result += std::to_wstring(arg); },
            [&](const std::string &arg) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              result += L"\"" + converter.from_bytes(arg) + L"\"";
            }},
        m_value);
    result += L")\n";
    return result;
  }

  llvm::Value *ConstantNode::GenerateCode(Context &context) const
  {
    return std::visit(
        [&](const auto &constant) -> llvm::Value * {
          using T = std::decay_t<decltype(constant)>;
          if constexpr (std::is_same_v<T, int32_t> ||
                        std::is_same_v<T, int64_t>)
          {
            return llvm::ConstantInt::get(type->GetIRType(context), constant,
                                          true);
          }
          else if constexpr (std::is_same_v<T, uint8_t> ||
                             std::is_same_v<T, uint32_t> ||
                             std::is_same_v<T, uint64_t>)
          {
            return llvm::ConstantInt::get(type->GetIRType(context), constant);
          }
          else if constexpr (std::is_same_v<T, float> ||
                             std::is_same_v<T, double>)
          {
            return llvm::ConstantFP::get(type->GetIRType(context), constant);
          }
          else if constexpr (std::is_same_v<T, std::string>)
          {
            llvm::ArrayType *arrayType = llvm::ArrayType::get(
                context.builder.getInt8Ty(), constant.length() + 1);
            llvm::Constant *cString = llvm::ConstantDataArray::getString(
                context.context, constant, true);
            llvm::GlobalVariable *cStringGlobal = new llvm::GlobalVariable(
                context.module, arrayType, true,
                llvm::GlobalValue::InternalLinkage, cString);
            return cStringGlobal;
          }

          return nullptr;
        },
        m_value);
  }

  std::unique_ptr<ConstantNode> ConstantNode::Parse(CLangLexer &lexer)
  {
    CLangToken nextToken = lexer.GetToken();
    switch (nextToken.type)
    {
    case CLangTokenType::IntegerConstant:
      return ParseIntegerConstant(nextToken);
    case CLangTokenType::FloatConstant:
      return ParseFloatConstant(nextToken);
    case CLangTokenType::CharacterConstant:
      return std::make_unique<ConstantNode>(
          std::make_unique<IntegralType>(Integral::Byte, false, false, false),
          (uint8_t)std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>()
              .to_bytes(nextToken.content)[0],
          nextToken.column, nextToken.line);
    case CLangTokenType::StringLiteral:
      return ParseStringLiteral(nextToken, lexer);
    default:
      throw CompileError(CompileErrorId::ExpectedConstant, nextToken.line,
                         nextToken.column);
    }
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseIntegerConstant(
      const CLangToken &token)
  {
    bool isUnsigned = false;
    int longCount = 0;

    int postfix = 0;
    auto iter = token.content.crbegin();
    while (iter != token.content.crend() && (*iter == 'u' || *iter == 'l'))
    {
      if (*iter == 'u')
      {
        isUnsigned = true;
      }
      else if (*iter == 'l')
      {
        longCount += 1;
      }
      postfix += 1;
      iter++;
    }

    int prefix = 0;
    int radix = 10;
    if (token.content.starts_with(L"0b"))
    {
      prefix = 2;
      radix = 2;
    }
    else if (token.content.starts_with(L"0x"))
    {
      prefix = 2;
      radix = 16;
    }
    else if (token.content.starts_with(L"0"))
    {
      prefix = 0;
      radix = 8;
    }

    std::wstring constant =
        token.content.substr(prefix, token.content.length() - postfix - prefix);

    uint64_t proxyValue;
    try
    {
      proxyValue = std::stoull(constant, nullptr, radix);
    }
    catch (...)
    {
      throw CompileError(CompileErrorId::ConstantTooLarge, token.column,
                         token.line, token.content);
    }

    bool valueParsed = false;
    ConstantValue value;
    while (!valueParsed)
    {
      if (isUnsigned && longCount >= 2)
      {
        value = proxyValue;
        valueParsed = true;
      }
      else if (longCount >= 2 &&
               proxyValue <= std::numeric_limits<int64_t>::max())
      {
        value = static_cast<int64_t>(proxyValue);
        valueParsed = true;
      }
      else if (isUnsigned && proxyValue <= std::numeric_limits<uint32_t>::max())
      {
        value = static_cast<uint32_t>(proxyValue);
        valueParsed = true;
      }
      else if (proxyValue <= std::numeric_limits<int32_t>::max())
      {
        value = static_cast<int32_t>(proxyValue);
        valueParsed = true;
      }

      if (!valueParsed && longCount < 2)
      {
        longCount = 2;
      }
      else if (!valueParsed)
      {
        isUnsigned = true;
      }
    }

    std::unique_ptr<StorageType> constantType = std::make_unique<IntegralType>(
        longCount < 2 ? Integral::Int : Integral::Long, isUnsigned, false,
        false);

    return std::make_unique<ConstantNode>(std::move(constantType), value,
                                          token.column, token.line);
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseFloatConstant(
      const CLangToken &token)
  {
    bool isDouble = true;

    int postfix = 0;
    auto iter = token.content.crbegin();
    while (iter != token.content.crend() && (*iter == 'f' || *iter == 'l'))
    {
      if (*iter == 'l')
      {
        isDouble = true;
      }
      else if (*iter == 'f')
      {
        isDouble = false;
      }
      postfix += 1;
      iter++;
    }

    std::wstring constant =
        token.content.substr(0, token.content.length() - postfix);

    std::unique_ptr<StorageType> constantType;
    ConstantValue value;
    bool valueParsed = false;
    while (!valueParsed)
    {
      try
      {
        if (isDouble)
        {
          constantType =
              std::make_unique<RealType>(Real::Double, TypeQualifiers(), false);
          value = std::stod(constant);
        }
        else
        {
          constantType =
              std::make_unique<RealType>(Real::Float, TypeQualifiers(), false);
          value = std::stof(constant);
        }
        valueParsed = true;
      }
      catch (...)
      {
        if (!isDouble)
        {
          isDouble = true;
        }
        else
        {
          throw CompileError(CompileErrorId::ConstantTooLarge, token.column,
                             token.line, token.content);
        }
      }
    }
    return std::make_unique<ConstantNode>(std::move(constantType), value,
                                          token.column, token.line);
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseStringLiteral(
      const CLangToken &token, CLangLexer &lexer)
  {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string stringLiteral = converter.to_bytes(token.content);
    while (lexer.PeekToken().type == CLangTokenType::StringLiteral)
    {
      CLangToken nextLiteral = lexer.GetToken();
      stringLiteral += converter.to_bytes(nextLiteral.content);
    }

    std::unique_ptr<StorageType> storageType = std::make_unique<IntegralType>(
        Integral::Byte, false, TypeQualifiers(true), false);
    std::unique_ptr<StorageType> pointerType =
        std::make_unique<PointerType>(storageType, false, false);
    return std::make_unique<ConstantNode>(std::move(pointerType), stringLiteral,
                                          token.column, token.line);
  }
} // namespace Vypr