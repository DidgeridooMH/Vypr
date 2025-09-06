#include "Vypr/AST/Expression/CastNode.hpp"

#include <gtest/gtest.h>

#include "Vypr/AST/SymbolTable.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

namespace CastNodeTest
{
  /*
   * Generate code
   * Casting types
   * - Integer 8/16/32/64
   *  - Integer 8/16/32/64
   *  - Unsigned Integer 8/16/32/64
   *  - Real 32/64
   *  - Pointer
   * - Unsigned Integer 8/16/32/64
   *  - Integer 8/16/32/64
   *  - Unsigned Integer 8/16/32/64
   *  - Real 32/64
   *  - Pointer
   * - Real 32/64
   *  - Integer 8/16/32/64
   *  - Unsigned Integer 8/16/32/64
   *  - Real 32/64
   *  - Pointer
   * - Pointer
   *  - Integer 64
   */

  TEST(CastNodeTestParse, Int8ToInt8)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"(char) 'c'"));
    Vypr::TypeTable typeTable;

    auto cast = Vypr::ExpressionNode::Parse(lexer, typeTable);

    ASSERT_EQ(lexer.PeekToken().type, Vypr::CLangTokenType::NoToken);
    ASSERT_NE(cast, nullptr);
    ASSERT_EQ(cast->column, 1);
    ASSERT_EQ(cast->line, 1);
    ASSERT_EQ(cast->type->GetType(), Vypr::StorageMetaType::Integral);

    auto integralType = dynamic_cast<Vypr::IntegralType *>(cast->type.get());
    ASSERT_EQ(integralType->integral, Vypr::Integral::Byte);
    ASSERT_FALSE(integralType->isConst);
    ASSERT_FALSE(integralType->isLValue);
    ASSERT_EQ(integralType->isUnsigned, false);
  }
} // namespace CastNodeTest