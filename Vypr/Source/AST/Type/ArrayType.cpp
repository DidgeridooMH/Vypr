#include "Vypr/AST/Type/ArrayType.hpp"

#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"

namespace Vypr
{
  ArrayType::ArrayType(std::unique_ptr<StorageType> &storage, size_t size,
                       int isLValue)
      : StorageType(StorageMetaType::Array, TypeQualifiers(), isLValue),
        m_storage(std::move(storage)), m_size(size)
  {
    qualifiers.isConst = true;
  }

  std::unique_ptr<StorageType> ArrayType::Clone() const
  {
    std::unique_ptr<StorageType> storageClone = m_storage->Clone();
    return std::make_unique<PointerType>(storageClone, TypeQualifiers(), false);
  }

  std::unique_ptr<StorageType> ArrayType::Check(PostfixOp op) const
  {
    return nullptr;
  }

  std::unique_ptr<StorageType> ArrayType::Check(UnaryOp op) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case UnaryOp::LogicalNot:
      resultType =
          std::make_unique<IntegralType>(Integral::Bool, false, false, false);
      break;
    case UnaryOp::Deref:
      break;
    case UnaryOp::AddressOf:
      break;
    case UnaryOp::Sizeof:
      break;
    default:
      break;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> ArrayType::Check(BinaryOp op,
                                                const StorageType &other) const
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
      if (other.GetType() == StorageMetaType::Void)
      {
        return nullptr;
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

  std::unique_ptr<StorageType> ArrayType::CheckArithmetic(
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

  std::wstring ArrayType::PrettyPrint() const
  {
    std::wstring result = StorageType::PrettyPrint();
    result += L"Array(";
    result += m_storage->PrettyPrint();
    result += L")";
    return result;
  }
} // namespace Vypr