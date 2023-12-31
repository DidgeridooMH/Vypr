#pragma once

namespace Vypr
{
  /// @brief Binary operations.
  enum class BinaryOp
  {
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    ShiftLeft,
    ShiftRight,
    And,
    Xor,
    Or,
    LessThan,
    LessEqual,
    GreaterThan,
    GreaterEqual,
    Equal,
    NotEqual,
    LogicalAnd,
    LogicalOr
  };
} // namespace Vypr