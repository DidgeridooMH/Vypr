#include "Vypr/AST/Type/RealType.hpp"

#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"

namespace Vypr
{
  RealType::RealType(Real real, TypeQualifiers qualifiers, bool isLValue)
      : StorageType(StorageMetaType::Real, qualifiers, isLValue), real(real)
  {
  }

  std::unique_ptr<StorageType> RealType::Clone() const
  {
    return std::make_unique<RealType>(real, qualifiers, false);
  }

  std::unique_ptr<StorageType> RealType::Check(PostfixOp op) const
  {
    return (isLValue && !qualifiers.isConst) ? Clone() : nullptr;
  }

  std::unique_ptr<StorageType> RealType::Check(UnaryOp op) const
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
    case UnaryOp::Negate:
      resultType = Clone();
      break;
    case UnaryOp::LogicalNot:
      resultType =
          std::make_unique<IntegralType>(Integral::Bool, false, false, false);
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
    default:
      break;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> RealType::Check(BinaryOp op,
                                               const StorageType &other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case BinaryOp::Add:
    case BinaryOp::Subtract:
    case BinaryOp::Multiply:
    case BinaryOp::Divide:
      resultType = CheckArithmetic(op, other);
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
    default:
      break;
    }

    if (resultType != nullptr)
    {
      resultType->isLValue = false;
      resultType->qualifiers.isConst = false;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> RealType::CheckArithmetic(
      BinaryOp op, const StorageType &other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (other.GetType())
    {
    case StorageMetaType::Integral:
      resultType = Clone();
      break;
    case StorageMetaType::Real:
      resultType = real >= dynamic_cast<const RealType *>(&other)->real
                       ? Clone()
                       : other.Clone();
      break;
    default:
      break;
    }

    return resultType;
  }

  llvm::Type *RealType::GetIRType(Context &context) const
  {
    if (real == Real::Float)
    {
      return context.builder.getFloatTy();
    }
    return context.builder.getDoubleTy();
  }

  std::wstring RealType::PrettyPrint() const
  {
    std::wstring result = StorageType::PrettyPrint();
    if (real == Real::Float)
    {
      result += L"Float";
    }
    else
    {
      result += L"Double";
    }
    return result;
  }
} // namespace Vypr