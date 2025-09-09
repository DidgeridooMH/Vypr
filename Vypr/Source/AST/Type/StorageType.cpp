#include "Vypr/AST/Type/StorageType.hpp"

#include "Vypr/AST/CompileError.hpp"

namespace Vypr
{
  StorageType::StorageType() : StorageType(StorageMetaType::Void, {}, false)
  {
  }

  StorageType::StorageType(StorageMetaType type, TypeQualifiers qualifiers,
                           bool isLValue)
      : m_type(type), qualifiers(qualifiers), isLValue(isLValue)
  {
  }

  std::unique_ptr<StorageType> StorageType::Clone() const
  {
    return std::make_unique<StorageType>(m_type, qualifiers, isLValue);
  };

  std::unique_ptr<StorageType> StorageType::Check(PostfixOp op) const
  {
    return nullptr;
  }

  std::unique_ptr<StorageType> StorageType::Check(UnaryOp op) const
  {
    return nullptr;
  }

  std::unique_ptr<StorageType> StorageType::Check(
      BinaryOp op, const StorageType &other) const
  {
    return nullptr;
  };

  std::wstring StorageType::PrettyPrint() const
  {
    std::wstring result;
    if (isLValue)
    {
      result += L"(L) ";
    }

    if (qualifiers.isConst)
    {
      result += L"const ";
    }

    if (qualifiers.isVolatile)
    {
      result += L"volatile ";
    }

    if (qualifiers.isRestrict)
    {
      result += L"restrict ";
    }

    if (qualifiers.isRegister)
    {
      result += L"register ";
    }

    switch (m_type)
    {
    case StorageMetaType::Integral:
      result += L"Integral";
      break;
    case StorageMetaType::Real:
      result += L"Real";
      break;
    case StorageMetaType::Pointer:
      result += L"Pointer";
      break;
    case StorageMetaType::Array:
      result += L"Array";
      break;
    case StorageMetaType::Void:
      result += L"Void";
      break;
    default:
      result += L"Unknown";
      break;
    }

    return result;
  }

  llvm::Type *StorageType::GetIRType(Context &context) const
  {
    if (m_type == StorageMetaType::Void)
    {
      return context.builder.getVoidTy();
    }
    throw CompileError(CompileErrorId::UnimplementedFeature, 0, 0);
  }

  StorageMetaType StorageType::GetType() const
  {
    return m_type;
  }

  std::unique_ptr<StorageType> StorageType::Parse(CLangLexer &lexer)
  {
    TypeQualifiers qualifiers;
    std::unique_ptr<StorageType> underlyingType = nullptr;

    while (true)
    {
    }
  }
} // namespace Vypr
