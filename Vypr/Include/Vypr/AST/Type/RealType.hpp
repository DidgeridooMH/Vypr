#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  enum class Real
  {
    Float,
    Double
  };

  class RealType : public StorageType
  {
  public:
    RealType(Real real, TypeQualifiers qualifiers, bool isLValue);

    std::unique_ptr<StorageType> Clone() const override;

    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType &other) const override;

    llvm::Type *GetIRType(Context &context) const override;

    std::wstring PrettyPrint() const override;

    Real real;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType &other) const;
  };

} // namespace Vypr