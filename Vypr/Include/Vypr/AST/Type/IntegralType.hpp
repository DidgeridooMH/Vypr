#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  /// @brief Size information for integral types.
  enum class Integral
  {
    /// @brief 1-bit value. Used for truthfulness where 1 is true and 0 is
    /// false.
    Bool,
    /// @brief 8-bit value
    Byte,
    /// @brief 16-bit value
    Short,
    /// @brief 32-bit value
    Int,
    /// @brief 64-bit value
    Long
  };

  /// @brief Type that holds integer values. These values can be sized as
  /// specified by `Integral` and can either be signed or unsigned.
  class IntegralType : public StorageType
  {
  public:
    /// @brief Constructs an integral type.
    ///
    /// @param integral Size of the integer.
    /// @param isUnsigned True if unsigned, false otherwise
    /// @param qualifiers Type qualifiers for the integral type.
    /// @param isLValue True if L-Value, false otherwise
    IntegralType(Integral integral, bool isUnsigned, TypeQualifiers qualifiers,
                 bool isLValue);

    /// @brief Creates a copy of the integer type.
    /// @return A deep copy of the integer type with `isLValue` flag not set.
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

    /// @brief Print the type in a readable format.
    /// @return Wide string containing the readable type.
    std::wstring PrettyPrint() const override;

    /// @brief Retrieve the mapping between the LLVM IR type and the current
    /// `integral`.
    /// @return An LLVM IR type associated with the integral type.
    llvm::Type *GetIRType(Context &context) const override;

    /// @brief Size of the integer.
    Integral integral;

    /// @brief True if the type is unsigned, false otherwise.
    bool isUnsigned;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType &other) const;

    std::unique_ptr<StorageType> CheckBitwise(BinaryOp op,
                                              const StorageType &other) const;
  };
} // namespace Vypr