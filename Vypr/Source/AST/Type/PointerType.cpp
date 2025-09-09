#include "Vypr/AST/Type/PointerType.hpp"

#include "Vypr/AST/Type/IntegralType.hpp"

namespace Vypr
{
  PointerType::PointerType(std::unique_ptr<StorageType> &storage,
                           TypeQualifiers qualifiers, bool isLValue)
      : StorageType(StorageMetaType::Pointer, qualifiers, isLValue),
        m_storage(std::move(storage))
  {
  }

  std::unique_ptr<StorageType> PointerType::Clone() const
  {
    std::unique_ptr<StorageType> storageClone = m_storage->Clone();
    return std::make_unique<PointerType>(storageClone, qualifiers.isConst,
                                         false);
  }

  std::unique_ptr<StorageType> PointerType::Check(PostfixOp op) const
  {
    return (isLValue && !qualifiers.isConst) ? Clone() : nullptr;
  }

  std::unique_ptr<StorageType> PointerType::Check(UnaryOp op) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case UnaryOp::Increment:
    case UnaryOp::Decrement:
      if (isLValue && !qualifiers.isConst)
      {
        resultType = Clone();
      }
      break;
    case UnaryOp::LogicalNot:
      resultType =
          std::make_unique<IntegralType>(Integral::Bool, false, false, false);
      break;
    case UnaryOp::Deref:
      // TODO: This will change to a nullptr check later.
      if (m_storage->GetType() != StorageMetaType::Void)
      {
        resultType = m_storage->Clone();
        resultType->qualifiers = m_storage->qualifiers;
        resultType->isLValue = true;
      }
      break;
    case UnaryOp::AddressOf:
      resultType = Clone();
      resultType = std::make_unique<PointerType>(resultType, false, false);
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

  std::unique_ptr<StorageType> PointerType::Check(
      BinaryOp op, const StorageType &other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case BinaryOp::Add:
    case BinaryOp::Subtract:
      resultType = CheckArithmetic(op, other);
      break;
    case BinaryOp::LessThan:
    case BinaryOp::LessEqual:
    case BinaryOp::GreaterThan:
    case BinaryOp::GreaterEqual:
    case BinaryOp::Equal:
    case BinaryOp::NotEqual:
      if (other.GetType() != StorageMetaType::Pointer &&
          other.GetType() != StorageMetaType::Array)
      {
        break;
      }
      [[fallthrough]];
    case BinaryOp::LogicalAnd:
    case BinaryOp::LogicalOr:
      resultType = std::make_unique<IntegralType>(Integral::Bool, false,
                                                  TypeQualifiers(), false);
      break;
    default:
      break;
    }

    return resultType;
  }

  std::wstring PointerType::PrettyPrint() const
  {
    std::wstring result = StorageType::PrettyPrint();
    result += L"Pointer(";
    result += m_storage->PrettyPrint();
    result += L")";
    return result;
  }

  std::unique_ptr<StorageType> PointerType::CheckArithmetic(
      BinaryOp op, const StorageType &other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (other.GetType())
    {
    case StorageMetaType::Integral:
      resultType = Clone();
      break;
    case StorageMetaType::Pointer:
    case StorageMetaType::Array:
      if (op == BinaryOp::Subtract)
      {
        resultType = std::make_unique<IntegralType>(Integral::Long, false,
                                                    TypeQualifiers(), false);
      }
      break;
    default:
      break;
    }

    return resultType;
  }

  llvm::Type *PointerType::GetIRType(Context &context) const
  {
    llvm::Type *result;
    if (m_storage != nullptr)
    {
      llvm::Type *storageType = m_storage->GetIRType(context);
      result = storageType->getPointerTo();
    }
    else
    {
      result = context.builder.getPtrTy();
    }
    return result;
  }
} // namespace Vypr