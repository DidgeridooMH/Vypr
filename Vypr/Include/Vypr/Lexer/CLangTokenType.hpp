#pragma once

namespace Vypr
{
  /// @brief Token types for the C language grammar.
  enum class CLangTokenType
  {
    NoToken,
    IntegerConstant,
    FloatConstant,
    CharacterConstant,
    StringLiteral,
    Identifier,
    LeftBracket,
    RightBracket,
    LeftParenthesis,
    RightParenthesis,
    LeftDragon,
    RightDragon,
    Period,
    Arrow,
    Increment,
    Decrement,
    Star,
    Add,
    Subtract,
    Tilde,
    Exclamation,
    Divide,
    Modulo,
    ShiftLeft,
    ShiftRight,
    LessThan,
    GreaterThan,
    LessEqual,
    GreaterEqual,
    Equal,
    NotEqual,
    And,
    Or,
    Xor,
    LogicalAnd,
    LogicalOr,
    TernaryProposition,
    TernaryDecision,
    StatementDelimiter,
    Variadic,
    Assign,
    MultiplyAssign,
    DivideAssign,
    ModuloAssign,
    AddAssign,
    SubtractAssign,
    LeftShiftAssign,
    RightShiftAssign,
    AndAssign,
    XorAssign,
    OrAssign,
    Auto,
    Comma,
    Preprocessor,
    PreprocessorConcat,
    Break,
    Case,
    CharType,
    DoubleType,
    IntegerType,
    FloatType,
    LongType,
    ShortType,
    Const,
    Continue,
    Default,
    Do,
    Else,
    Enumeration,
    Extern,
    For,
    Goto,
    If,
    Inline,
    Register,
    Restrict,
    Return,
    Signed,
    Sizeof,
    Static,
    Struct,
    Switch,
    Typedef,
    Union,
    Unsigned,
    Void,
    Volatile,
    While,
    AlignAs,
    AlignOf,
    Atomic,
    Boolean,
    Complex,
    Generic,
    Imaginary,
    NoReturn,
    StaticAssert,
    ThreadLocal,
  };
} // namespace Vypr