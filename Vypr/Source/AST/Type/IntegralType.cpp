#include "Vypr/AST/Type/IntegralType.hpp"

#include <unordered_map>

#include "Vypr/AST/Type/PointerType.hpp"

namespace Vypr
{
  IntegralType::IntegralType(Integral integral, bool isUnsigned,
                             TypeQualifiers qualifiers, bool isLValue)
      : StorageType(StorageMetaType::Integral, qualifiers, isLValue),
        integral(integral), isUnsigned(isUnsigned)
  {
  }

  std::unique_ptr<StorageType> IntegralType::Clone() const
  {
    return std::make_unique<IntegralType>(integral, isUnsigned,
                                          TypeQualifiers(), false);
  }

  std::unique_ptr<StorageType> IntegralType::Check(PostfixOp op) const
  {
    return (isLValue && !qualifiers.isConst) ? Clone() : nullptr;
  }

  std::unique_ptr<StorageType> IntegralType::Check(UnaryOp op) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case UnaryOp::Increment:
    case UnaryOp::Decrement:
      if (isLValue)
      {
        resultType = Clone();
      }
      break;
    case UnaryOp::Not:
    case UnaryOp::Negate:
      if (integral == Integral::Bool)
      {
        resultType =
            std::make_unique<IntegralType>(Integral::Int, false, false, false);
      }
      else
      {
        resultType = Clone();
      }
      break;
    case UnaryOp::LogicalNot:
      resultType = std::make_unique<IntegralType>(Integral::Bool, false,
                                                  TypeQualifiers(), false);
      break;
    case UnaryOp::Deref:
      break;
    case UnaryOp::AddressOf:
      if (isLValue)
      {
        resultType = Clone();
        resultType = std::make_unique<PointerType>(resultType, false, false);
      }
      break;
    case UnaryOp::Sizeof:
      resultType = std::make_unique<IntegralType>(Integral::Long, true,
                                                  TypeQualifiers(), false);
      break;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> IntegralType::Check(
      BinaryOp op, const StorageType &other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case BinaryOp::Add:
    case BinaryOp::Subtract:
    case BinaryOp::Multiply:
    case BinaryOp::Divide:
    case BinaryOp::Modulo:
      resultType = CheckArithmetic(op, other);
      break;
    case BinaryOp::ShiftLeft:
    case BinaryOp::ShiftRight:
    case BinaryOp::And:
    case BinaryOp::Xor:
    case BinaryOp::Or:
      resultType = CheckBitwise(op, other);
      break;
    case BinaryOp::LessThan:
    case BinaryOp::LessEqual:
    case BinaryOp::GreaterThan:
    case BinaryOp::GreaterEqual:
    case BinaryOp::Equal:
    case BinaryOp::NotEqual:
    case BinaryOp::LogicalAnd:
    case BinaryOp::LogicalOr:
      resultType = std::make_unique<IntegralType>(Integral::Bool, false,
                                                  TypeQualifiers(), false);
      break;
    }

    if (resultType != nullptr)
    {
      resultType->isLValue = false;
      resultType->qualifiers.isConst = false;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> IntegralType::CheckArithmetic(
      BinaryOp op, const StorageType &other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (other.GetType())
    {
    case StorageMetaType::Integral:
      resultType =
          integral >= dynamic_cast<const IntegralType *>(&other)->integral
              ? Clone()
              : other.Clone();
      dynamic_cast<IntegralType *>(resultType.get())->isUnsigned =
          isUnsigned || dynamic_cast<const IntegralType *>(&other)->isUnsigned;
      break;
    case StorageMetaType::Real:
      if (op != BinaryOp::Modulo)
      {
        resultType = other.Clone();
      }
      break;
    case StorageMetaType::Pointer:
    case StorageMetaType::Array:
      if (op == BinaryOp::Add)
      {
        resultType = other.Clone();
      }
      break;
    default:
      break;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> IntegralType::CheckBitwise(
      BinaryOp op, const StorageType &other) const
  {
    if (other.GetType() != StorageMetaType::Integral)
    {
      return nullptr;
    }

    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case BinaryOp::ShiftLeft:
    case BinaryOp::ShiftRight:
      resultType = Clone();
      break;
    case BinaryOp::And:
    case BinaryOp::Xor:
    case BinaryOp::Or:
      resultType =
          integral >= dynamic_cast<const IntegralType *>(&other)->integral
              ? Clone()
              : other.Clone();
      dynamic_cast<IntegralType *>(resultType.get())->isUnsigned =
          isUnsigned || dynamic_cast<const IntegralType *>(&other)->isUnsigned;
      break;
    default:
      break;
    }

    return resultType;
  }

  llvm::Type *IntegralType::GetIRType(Context &context) const
  {
    static const std::unordered_map<Integral,
                                    llvm::IntegerType *(llvm::IRBuilder<>::*)()>
        TypeBuilders = {{Integral::Byte, &llvm::IRBuilder<>::getInt8Ty},
                        {Integral::Short, &llvm::IRBuilder<>::getInt16Ty},
                        {Integral::Int, &llvm::IRBuilder<>::getInt32Ty},
                        {Integral::Long, &llvm::IRBuilder<>::getInt64Ty}};

    return (context.builder.*(TypeBuilders.at(integral)))();
  }

  std::wstring IntegralType::PrettyPrint() const
  {
    static const std::unordered_map<Integral, std::wstring> PrimitiveTypeNames =
        {{Integral::Bool, L"Bool"},
         {Integral::Byte, L"Byte"},
         {Integral::Short, L"Short"},
         {Integral::Int, L"Int"},
         {Integral::Long, L"Long"}};

    std::wstring result = StorageType::PrettyPrint();
    if (isUnsigned)
    {
      result += L"U";
    }
    result += PrimitiveTypeNames.at(integral);
    return result;
  }
} // namespace Vypr