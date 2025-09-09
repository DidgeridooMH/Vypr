#include "Vypr/AST/Expression/BinaryOpNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/CastNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"
#include "Vypr/AST/Type/RealType.hpp"

namespace Vypr
{
  const std::unordered_map<CLangTokenType, BinaryOp> BinaryOperations = {
      {CLangTokenType::Add, BinaryOp::Add},
      {CLangTokenType::Subtract, BinaryOp::Subtract},
      {CLangTokenType::Star, BinaryOp::Multiply},
      {CLangTokenType::Divide, BinaryOp::Divide},
      {CLangTokenType::Modulo, BinaryOp::Modulo},
      {CLangTokenType::ShiftLeft, BinaryOp::ShiftLeft},
      {CLangTokenType::ShiftRight, BinaryOp::ShiftRight},
      {CLangTokenType::LessThan, BinaryOp::LessThan},
      {CLangTokenType::LessEqual, BinaryOp::LessEqual},
      {CLangTokenType::GreaterThan, BinaryOp::GreaterThan},
      {CLangTokenType::GreaterEqual, BinaryOp::GreaterEqual},
      {CLangTokenType::Equal, BinaryOp::Equal},
      {CLangTokenType::NotEqual, BinaryOp::NotEqual},
      {CLangTokenType::And, BinaryOp::And},
      {CLangTokenType::Xor, BinaryOp::Xor},
      {CLangTokenType::Or, BinaryOp::Or},
      {CLangTokenType::LogicalAnd, BinaryOp::LogicalAnd},
      {CLangTokenType::LogicalOr, BinaryOp::LogicalOr}};

  const std::unordered_map<BinaryOp, int> BinaryOperationPrecedence = {
      {BinaryOp::Add, 3},          {BinaryOp::Subtract, 3},
      {BinaryOp::Multiply, 2},     {BinaryOp::Divide, 2},
      {BinaryOp::Modulo, 2},       {BinaryOp::ShiftLeft, 4},
      {BinaryOp::ShiftRight, 4},   {BinaryOp::LessThan, 5},
      {BinaryOp::LessEqual, 5},    {BinaryOp::GreaterThan, 5},
      {BinaryOp::GreaterEqual, 5}, {BinaryOp::Equal, 6},
      {BinaryOp::NotEqual, 6},     {BinaryOp::And, 7},
      {BinaryOp::Xor, 8},          {BinaryOp::Or, 9},
      {BinaryOp::LogicalAnd, 10},  {BinaryOp::LogicalOr, 11}};

  const std::unordered_map<BinaryOp, std::wstring> BinaryOperationNames = {
      {BinaryOp::Add, L"Add"},
      {BinaryOp::Subtract, L"Subtract"},
      {BinaryOp::Multiply, L"Multiply"},
      {BinaryOp::Divide, L"Divide"},
      {BinaryOp::Modulo, L"Modulo"},
      {BinaryOp::ShiftLeft, L"ShiftLeft"},
      {BinaryOp::ShiftRight, L"ShiftRight"},
      {BinaryOp::LessThan, L"LessThan"},
      {BinaryOp::LessEqual, L"LessEqual"},
      {BinaryOp::GreaterThan, L"GreaterThan"},
      {BinaryOp::GreaterEqual, L"GreaterEqual"},
      {BinaryOp::Equal, L"Equal"},
      {BinaryOp::NotEqual, L"NotEqual"},
      {BinaryOp::And, L"And"},
      {BinaryOp::Xor, L"Xor"},
      {BinaryOp::Or, L"Or"},
      {BinaryOp::LogicalAnd, L"LogicalAnd"},
      {BinaryOp::LogicalOr, L"LogicalOr"}};

  BinaryOpNode::BinaryOpNode(BinaryOp op, std::unique_ptr<ExpressionNode> &&lhs,
                             std::unique_ptr<ExpressionNode> &&rhs,
                             size_t column, size_t line)
      : ExpressionNode(nullptr, column, line), m_op(op), m_lhs(std::move(lhs)),
        m_rhs(std::move(rhs))
  {
    type = m_lhs->type->Check(op, *m_rhs->type);
    if (type == nullptr)
    {
      throw CompileError(CompileErrorId::InvalidOperands, column, line);
    }
    CastType();
  }

  void BinaryOpNode::CastType()
  {
    switch (m_op)
    {
    case BinaryOp::Add:
    case BinaryOp::Subtract:
    case BinaryOp::Multiply:
    case BinaryOp::Divide:
    case BinaryOp::Modulo:
      CastTypeArithmetic(m_op);
      break;
    case BinaryOp::And:
    case BinaryOp::Xor:
    case BinaryOp::Or:
      CastTypeBitwise(m_op);
      break;
    case BinaryOp::LessThan:
    case BinaryOp::LessEqual:
    case BinaryOp::GreaterThan:
    case BinaryOp::GreaterEqual:
    case BinaryOp::Equal:
    case BinaryOp::NotEqual:
      CastTypeComparison(m_op);
      break;
    case BinaryOp::LogicalAnd:
    case BinaryOp::LogicalOr:
      CastTypeLogic(m_op);
      break;
    default:
      break;
    }
  }

  void BinaryOpNode::CastTypeIntegral()
  {
    IntegralType *exprType = dynamic_cast<IntegralType *>(type.get());
    IntegralType *lhs = dynamic_cast<IntegralType *>(m_lhs->type.get());

    if (lhs->integral < exprType->integral ||
        lhs->isUnsigned != exprType->isUnsigned)
    {
      m_lhs = std::make_unique<CastNode>(exprType->Clone(), std::move(m_lhs));
    }

    IntegralType *rhs = dynamic_cast<IntegralType *>(m_rhs->type.get());
    if (rhs->integral < exprType->integral ||
        rhs->isUnsigned != exprType->isUnsigned)
    {
      m_rhs = std::make_unique<CastNode>(exprType->Clone(), std::move(m_rhs));
    }
  }

  void BinaryOpNode::CastTypeIntegralComparison()
  {
    IntegralType *lhs = dynamic_cast<IntegralType *>(m_lhs->type.get());
    IntegralType *rhs = dynamic_cast<IntegralType *>(m_rhs->type.get());

    if (lhs->integral < rhs->integral)
    {
      std::unique_ptr<StorageType> castType = m_rhs->type->Clone();
      dynamic_cast<IntegralType *>(castType.get())->isUnsigned |=
          rhs->isUnsigned;
      m_lhs = std::make_unique<CastNode>(std::move(castType), std::move(m_lhs));
    }

    if (rhs->integral < lhs->integral)
    {
      std::unique_ptr<StorageType> castType = lhs->Clone();
      dynamic_cast<IntegralType *>(castType.get())->isUnsigned |=
          rhs->isUnsigned;
      m_rhs = std::make_unique<CastNode>(std::move(castType), std::move(m_rhs));
    }
  }

  void BinaryOpNode::CastTypeReal()
  {
    if (m_rhs->type->GetType() != StorageMetaType::Real ||
        (dynamic_cast<RealType *>(m_lhs.get())->real >
         dynamic_cast<RealType *>(m_rhs.get())->real))
    {
      m_rhs =
          std::make_unique<CastNode>(m_lhs->type->Clone(), std::move(m_rhs));
    }
    else if (m_lhs->type->GetType() != StorageMetaType::Real ||
             ((dynamic_cast<RealType *>(m_lhs.get())->real <
               dynamic_cast<RealType *>(m_rhs.get())->real)))
    {
      m_lhs =
          std::make_unique<CastNode>(m_rhs->type->Clone(), std::move(m_lhs));
    }
  }

  void BinaryOpNode::CastTypePointer()
  {
    if (m_lhs->type->GetType() == StorageMetaType::Array)
    {
      m_lhs =
          std::make_unique<CastNode>(m_lhs->type->Clone(), std::move(m_lhs));
    }
    else if (m_lhs->type->GetType() != StorageMetaType::Pointer)
    {
      std::unique_ptr<StorageType> voidType = std::make_unique<StorageType>();
      m_lhs = std::make_unique<CastNode>(
          std::make_unique<PointerType>(voidType, false, false),
          std::move(m_lhs));
    }

    if (m_rhs->type->GetType() == StorageMetaType::Array)
    {
      m_rhs =
          std::make_unique<CastNode>(m_rhs->type->Clone(), std::move(m_rhs));
    }
    else if (m_rhs->type->GetType() != StorageMetaType::Pointer)
    {
      std::unique_ptr<StorageType> voidType = std::make_unique<StorageType>();
      m_rhs = std::make_unique<CastNode>(
          std::make_unique<PointerType>(voidType, TypeQualifiers(), false),
          std::move(m_rhs));
    }
  }

  void BinaryOpNode::CastTypeArithmetic(BinaryOp op)
  {
    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      CastTypeIntegral();
      break;
    case StorageMetaType::Real:
      CastTypeReal();
      break;
    default:
      throw CompileError(CompileErrorId::UnimplementedFeature, 0, 0);
    }
  }

  void BinaryOpNode::CastTypeBitwise(BinaryOp op)
  {
    CastTypeIntegral();
  }

  // TODO: This will always be bool since comparisons are bool
  void BinaryOpNode::CastTypeComparison(BinaryOp op)
  {
    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      CastTypeIntegralComparison();
      break;
    case StorageMetaType::Array:
    case StorageMetaType::Pointer:
      CastTypePointer();
      break;
    default:
      break;
    }
  }

  void BinaryOpNode::CastTypeLogic(BinaryOp op)
  {
    if (m_lhs->type->GetType() != StorageMetaType::Integral ||
        dynamic_cast<IntegralType *>(m_lhs->type.get())->integral !=
            Integral::Bool)
    {
      m_lhs = std::make_unique<CastNode>(
          std::make_unique<IntegralType>(Integral::Bool, false, false, false),
          std::move(m_lhs));
    }

    if (m_rhs->type->GetType() != StorageMetaType::Integral ||
        dynamic_cast<IntegralType *>(m_rhs->type.get())->integral !=
            Integral::Bool)
    {
      m_rhs = std::make_unique<CastNode>(
          std::make_unique<IntegralType>(Integral::Bool, false,
                                         TypeQualifiers(), false),
          std::move(m_rhs));
    }
  }

  std::wstring BinaryOpNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"BinaryOpNode(" + BinaryOperationNames.at(m_op) + L")\n";
    result += m_lhs->PrettyPrint(level + 1);
    result += m_rhs->PrettyPrint(level + 1);
    return result;
  }

  std::unique_ptr<ExpressionNode> BinaryOpNode::Parse(
      std::unique_ptr<ExpressionNode> &base, CLangLexer &lexer,
      const ASTContext &context)
  {
    CLangToken opToken = lexer.GetToken();
    BinaryOp op = BinaryOperations.at(opToken.type);

    std::unique_ptr<ExpressionNode> rhs =
        ExpressionNode::Parse(lexer, context, BinaryOperationPrecedence.at(op));
    if (rhs == nullptr)
    {
      throw CompileError(CompileErrorId::ExpectedExpression, opToken.column,
                         opToken.line);
    }

    return std::make_unique<BinaryOpNode>(op, std::move(base), std::move(rhs),
                                          opToken.column, opToken.line);
  }

  llvm::Value *BinaryOpNode::GenerateCode(Context &context) const
  {
    llvm::Value *lhs = m_lhs->GenerateCode(context);
    llvm::Value *rhs = m_rhs->GenerateCode(context);

    switch (m_op)
    {
    case BinaryOp::Add:
      if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateAdd(lhs, rhs);
      }
      return context.builder.CreateFAdd(lhs, rhs);
    case BinaryOp::Subtract:
      if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateSub(lhs, rhs);
      }
      return context.builder.CreateFSub(lhs, rhs);
    case BinaryOp::Multiply:
      if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateMul(lhs, rhs);
      }
      return context.builder.CreateFMul(lhs, rhs);
    case BinaryOp::Divide:
      if (type->GetType() == StorageMetaType::Integral &&
          dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        return context.builder.CreateUDiv(lhs, rhs);
      }
      else if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateSDiv(lhs, rhs);
      }
      return context.builder.CreateFDiv(lhs, rhs);
    case BinaryOp::Modulo:
      if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        return context.builder.CreateURem(lhs, rhs);
      }
      return context.builder.CreateSRem(lhs, rhs);
    case BinaryOp::LogicalAnd:
    case BinaryOp::And:
      return context.builder.CreateAnd(lhs, rhs);
    case BinaryOp::Xor:
      return context.builder.CreateXor(lhs, rhs);
    case BinaryOp::LogicalOr:
    case BinaryOp::Or:
      return context.builder.CreateOr(lhs, rhs);
    case BinaryOp::LessThan:
      if (type->GetType() == StorageMetaType::Integral &&
          dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        return context.builder.CreateICmpSLT(lhs, rhs);
      }
      else if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateICmpULT(lhs, rhs);
      }
      return context.builder.CreateFCmpOLT(lhs, rhs);
      break;
    case BinaryOp::LessEqual:
      if (type->GetType() == StorageMetaType::Integral &&
          dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        return context.builder.CreateICmpSLE(lhs, rhs);
      }
      else
      {
        return context.builder.CreateICmpULE(lhs, rhs);
      }
      return context.builder.CreateFCmpOLE(lhs, rhs);
    case BinaryOp::GreaterThan:
      if (type->GetType() == StorageMetaType::Integral &&
          dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        return context.builder.CreateICmpSGT(lhs, rhs);
      }
      else if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateICmpUGT(lhs, rhs);
      }
      return context.builder.CreateFCmpOGT(lhs, rhs);
    case BinaryOp::GreaterEqual:
      if (type->GetType() == StorageMetaType::Integral &&
          dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        return context.builder.CreateICmpSGE(lhs, rhs);
      }
      else if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateICmpUGE(lhs, rhs);
      }
      return context.builder.CreateFCmpOGE(lhs, rhs);
    case BinaryOp::Equal:
      if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateICmpEQ(lhs, rhs);
      }
      return context.builder.CreateFCmpOEQ(lhs, rhs);
    case BinaryOp::NotEqual:
      if (type->GetType() == StorageMetaType::Integral)
      {
        return context.builder.CreateICmpNE(lhs, rhs);
      }
      return context.builder.CreateFCmpONE(lhs, rhs);
    default:
      break;
    }

    throw CompileError(CompileErrorId::UnimplementedFeature, 0, 0);
  }
} // namespace Vypr