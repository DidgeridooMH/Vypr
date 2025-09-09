#include "Vypr/Lexer/CLangTokenMap.hpp"

namespace Vypr
{
  const CLangTokenSet TypePrefixSet = {
      CLangTokenType::CharType,    CLangTokenType::ShortType,
      CLangTokenType::IntegerType, CLangTokenType::LongType,
      CLangTokenType::FloatType,   CLangTokenType::DoubleType,
      CLangTokenType::Boolean,     CLangTokenType::Struct,
      CLangTokenType::Enumeration, CLangTokenType::Unsigned,
      CLangTokenType::Const,       CLangTokenType::Signed,
      CLangTokenType::Void,        CLangTokenType::Restrict,
      CLangTokenType::Register,    CLangTokenType::Volatile};
} // namespace Vypr
