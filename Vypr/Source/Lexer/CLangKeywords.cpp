#include "Vypr/Lexer/CLangTokenMap.hpp"

namespace Vypr
{
  const CLangTokenMap KeywordMap = {
    { L"auto", CLangTokenType::Auto },
    { L"break", CLangTokenType::Break },
    { L"case", CLangTokenType::Case },
    { L"char", CLangTokenType::CharType },
    { L"double", CLangTokenType::DoubleType },
    { L"int", CLangTokenType::IntegerType },
    { L"float", CLangTokenType::FloatType },
    { L"long", CLangTokenType::LongType },
    { L"short", CLangTokenType::ShortType },
    { L"const", CLangTokenType::Const },
    { L"continue", CLangTokenType::Continue },
    { L"default", CLangTokenType::Default },
    { L"do", CLangTokenType::Do },
    { L"else", CLangTokenType::Else },
    { L"enum", CLangTokenType::Enumeration },
    { L"extern", CLangTokenType::Extern },
    { L"for", CLangTokenType::For },
    { L"goto", CLangTokenType::Goto },
    { L"if", CLangTokenType::If },
    { L"inline", CLangTokenType::Inline },
    { L"register", CLangTokenType::Register },
    { L"restrict", CLangTokenType::Restrict },
    { L"return", CLangTokenType::Return },
    { L"signed", CLangTokenType::Signed },
    { L"sizeof", CLangTokenType::Sizeof },
    { L"static", CLangTokenType::Static },
    { L"struct", CLangTokenType::Struct },
    { L"switch", CLangTokenType::Switch },
    { L"typedef", CLangTokenType::Typedef },
    { L"union", CLangTokenType::Union },
    { L"unsigned", CLangTokenType::Unsigned },
    { L"void", CLangTokenType::Void },
    { L"volatile", CLangTokenType::Volatile },
    { L"while", CLangTokenType::While },
    { L"_Alignas", CLangTokenType::AlignAs },
    { L"_Alignof", CLangTokenType::AlignOf },
    { L"_Atomic", CLangTokenType::Atomic },
    { L"_Bool", CLangTokenType::Boolean },
    { L"_Complex", CLangTokenType::Complex },
    { L"_Generic", CLangTokenType::Generic },
    { L"_Imaginary", CLangTokenType::Imaginary },
    { L"_Noreturn", CLangTokenType::NoReturn },
    { L"_Static_assert", CLangTokenType::StaticAssert },
    { L"_Thread_local", CLangTokenType::ThreadLocal }
  };
}