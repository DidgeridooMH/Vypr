#include "Vypr/AST/Expression/CastNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/RealType.hpp"
#include "llvm/IR/Constants.h"

namespace Vypr
{
  CastNode::CastNode(std::unique_ptr<StorageType> &&castType,
                     std::unique_ptr<ExpressionNode> &&expression)
      : ExpressionNode(std::move(castType), expression->column,
                       expression->line),
        expression(std::move(expression))
  {
  }

  std::wstring CastNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"CastNode\n";
    result += expression->PrettyPrint(level + 1);
    return result;
  }

  std::unique_ptr<CastNode> CastNode::Parse(CLangLexer &lexer,
                                            const ASTContext &context)
  {
    // @todo: Parse type outside of this function. It will be useful later in
    // declarations.
    std::unique_ptr<StorageType> castType = nullptr;
    auto token = lexer.GetToken();
    if (token.type == CLangTokenType::Void)
    {
      castType = std::make_unique<StorageType>();
    }
    else
    {
      throw CompileError(CompileErrorId::UnimplementedFeature, token.line,
                         token.column, token.content);
    }

    token = lexer.GetToken();
    if (token.type != CLangTokenType::RightParenthesis)
    {
      throw CompileError(CompileErrorId::ExpectedGroupEnd, token.line,
                         token.column, token.content);
    }

    auto innerExpression = ExpressionNode::Parse(lexer, context);

    return std::make_unique<CastNode>(std::move(castType),
                                      std::move(innerExpression));
  }

  llvm::Value *CastNode::GenerateCode(Context &context) const
  {
    llvm::Value *resultValue = expression->GenerateCode(context);

    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      return CastToIntegral(context, resultValue);
    case StorageMetaType::Real:
      return CastToReal(context, resultValue);
    case StorageMetaType::Pointer:
      return CastToPointer(context, resultValue);
    default:
      return llvm::UndefValue::get(context.builder.getVoidTy());
    }

    throw CompileError(CompileErrorId::InvalidCast, 0, 0);
  }

  llvm::Value *CastNode::CastToIntegral(Context &context,
                                        llvm::Value *childExpression) const
  {
    switch (expression->type->GetType())
    {
    case StorageMetaType::Integral:
      return CastIntegralToIntegral(context, childExpression);
    case StorageMetaType::Pointer:
      if (dynamic_cast<IntegralType *>(type.get())->integral != Integral::Long)
      {
        throw CompileError(CompileErrorId::InvalidCast, 0, 0);
      }
      return context.builder.CreatePtrToInt(childExpression,
                                            type->GetIRType(context));
    case StorageMetaType::Real:
      return context.builder.CreateFPCast(childExpression,
                                          type->GetIRType(context));
    default:
      // TODO: Void is not checked here as the implementation will change at
      // some point.
      break;
    }

    throw CompileError(CompileErrorId::UnimplementedFeature, 0, 0);
  }

  llvm::Value *CastNode::CastToReal(Context &context,
                                    llvm::Value *childExpression) const
  {
    switch (expression->type->GetType())
    {
    case StorageMetaType::Integral:
      if (dynamic_cast<IntegralType *>(expression->type.get())->isUnsigned)
      {
        return context.builder.CreateUIToFP(childExpression,
                                            type->GetIRType(context));
      }
      return context.builder.CreateSIToFP(childExpression,
                                          type->GetIRType(context));
    case StorageMetaType::Real:
      if (dynamic_cast<RealType *>(type.get())->real <
          dynamic_cast<RealType *>(expression->type.get())->real)
      {
        return context.builder.CreateFPTrunc(childExpression,
                                             type->GetIRType(context));
      }
      return context.builder.CreateFPExt(childExpression,
                                         type->GetIRType(context));
    default:
      throw CompileError(CompileErrorId::InvalidCast, 0, 0);
    }
  }

  llvm::Value *CastNode::CastToPointer(Context &context,
                                       llvm::Value *childExpression) const
  {
    if (expression->type->GetType() != StorageMetaType::Integral ||
        dynamic_cast<IntegralType *>(expression->type.get())->integral !=
            Integral::Long)
    {
      throw CompileError(CompileErrorId::InvalidCast, 0, 0);
    }

    return context.builder.CreateIntToPtr(childExpression,
                                          type->GetIRType(context));
  }

  llvm::Value *CastNode::CastIntegralToIntegral(
      Context &context, llvm::Value *childExpression) const
  {
    auto integralType = dynamic_cast<IntegralType *>(type.get());
    if (integralType->integral == Integral::Bool)
    {
      return context.builder.CreateICmpNE(
          childExpression,
          llvm::ConstantInt::get(context.builder.getInt1Ty(), 0));
    }
    else if (integralType->isUnsigned)
    {
      return context.builder.CreateZExtOrTrunc(childExpression,
                                               type->GetIRType(context));
    }
    return context.builder.CreateSExtOrTrunc(childExpression,
                                             type->GetIRType(context));
  }
} // namespace Vypr
