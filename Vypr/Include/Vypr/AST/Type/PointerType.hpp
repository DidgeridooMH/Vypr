#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  /// @brief Type that denotes a pointer to another storage type. The stored
  /// type can be any of the derivations of `StorageType`.
  class PointerType : public StorageType
  {
  public:
    /// @brief Constructs a type that points to another type.
    ///
    /// @param storage Type pointed to by this pointer type.
    /// @param qualifiers Type qualifiers for the pointer type.
    /// @param isLValue True if L-Value, false otherwise.
    PointerType(std::unique_ptr<StorageType> &storage,
                TypeQualifiers qualifiers, bool isLValue);

    /// @brief Creates a copy of the pointer type.
    /// @return A deep copy of the pointer type with `isLValue` flag not set.
    std::unique_ptr<StorageType> Clone() const override;

    /// @brief Checks if a postfix operation is valid on the current type.
    /// @param op Postfix operation to check.
    /// @return Type of the result of the postfix operation.
    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    /// @brief Checks if a unary operation is valid on the current type.
    /// @param op Unary operation to check.
    /// @return Type of the result of the unary operation.
    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    /// @brief Checks if a binary operation is valid on the current type and
    /// operand.
    /// @param op Binary operation to check.
    /// @param other Operand to check on right side of binary operation.
    /// @return Type of the result of the binary operation.
    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType &other) const override;

    /// @brief Retrieve the mapping between the LLVM IR type and the current
    /// pointer to a storage type.
    /// @return An LLVM IR type associated with the pointer to storage type.
    llvm::Type *GetIRType(Context &context) const override;

    /// @brief Print the type in a readable format.
    /// @return Wide string containing the readable type.
    std::wstring PrettyPrint() const override;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType &other) const;

    std::unique_ptr<StorageType> m_storage;
  };
} // namespace Vypr