#include "Vypr/AST/Expression/ConstantNode.hpp"

#include <gtest/gtest.h>

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"
#include "Vypr/AST/Type/RealType.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

namespace ConstantNodeTest
{
  TEST(Parse, InvalidConstant)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"!"));
    ASSERT_THROW(Vypr::ConstantNode::Parse(lexer), Vypr::CompileError);
  }

#define PARSE_INT_TEST(testName, content, integralType, sign)                  \
  TEST(Parse, testName)                                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L##content)); \
                                                                               \
    std::unique_ptr<Vypr::ConstantNode> constant =                             \
        Vypr::ConstantNode::Parse(lexer);                                      \
                                                                               \
    ASSERT_EQ(lexer.PeekToken().type, Vypr::CLangTokenType::NoToken);          \
    ASSERT_NE(constant, nullptr);                                              \
    ASSERT_EQ(constant->column, 1);                                            \
    ASSERT_EQ(constant->line, 1);                                              \
    ASSERT_EQ(constant->type->GetType(), Vypr::StorageMetaType::Integral);     \
                                                                               \
    auto integralType =                                                        \
        dynamic_cast<Vypr::IntegralType *>(constant->type.get());              \
    ASSERT_EQ(integralType->integral, Vypr::Integral::integralType);           \
    ASSERT_FALSE(integralType->qualifiers.isConst);                            \
    ASSERT_FALSE(integralType->isLValue);                                      \
    ASSERT_EQ(integralType->isUnsigned, sign);                                 \
  }

  PARSE_INT_TEST(ZeroSignedInt, "0", Int, false);
  PARSE_INT_TEST(ZeroUnsignedInt, "0U", Int, true);
  PARSE_INT_TEST(ZeroSignedLong, "0L", Int, false);
  PARSE_INT_TEST(ZeroUnsignedLong, "0UL", Int, true);
  PARSE_INT_TEST(ZeroSignedLongLong, "0LL", Long, false);
  PARSE_INT_TEST(ZeroUnsignedLongLong, "0ULL", Long, true);
  PARSE_INT_TEST(OneSignedInt, "1", Int, false);
  PARSE_INT_TEST(OneUnsignedInt, "1U", Int, true);
  PARSE_INT_TEST(OneSignedLong, "1L", Int, false);
  PARSE_INT_TEST(OneUnsignedLong, "1UL", Int, true);
  PARSE_INT_TEST(OneSignedLongLong, "1LL", Long, false);
  PARSE_INT_TEST(OneUnsignedLongLong, "1ULL", Long, true);
  PARSE_INT_TEST(MultipleDigitSignedInt, "123", Int, false);
  PARSE_INT_TEST(MultipleDigitUnsignedInt, "123U", Int, true);
  PARSE_INT_TEST(MultipleDigitSignedLong, "123L", Int, false);
  PARSE_INT_TEST(MultipleDigitUnsignedLong, "123UL", Int, true);
  PARSE_INT_TEST(MultipleDigitSignedLongLong, "123LL", Long, false);
  PARSE_INT_TEST(MultipleDigitUnsignedLongLong, "123ULL", Long, true);

  PARSE_INT_TEST(HexInt, "0xFF", Int, false);
  PARSE_INT_TEST(BinaryInt, "0b010", Int, false);

#define PARSE_FLOAT_TEST(testName, content, realType)                          \
  TEST(Parse, testName)                                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L##content)); \
                                                                               \
    std::unique_ptr<Vypr::ConstantNode> constant =                             \
        Vypr::ConstantNode::Parse(lexer);                                      \
                                                                               \
    ASSERT_EQ(lexer.PeekToken().type, Vypr::CLangTokenType::NoToken);          \
    ASSERT_NE(constant, nullptr);                                              \
    ASSERT_EQ(constant->column, 1);                                            \
    ASSERT_EQ(constant->line, 1);                                              \
    ASSERT_EQ(constant->type->GetType(), Vypr::StorageMetaType::Real);         \
                                                                               \
    auto realType = dynamic_cast<Vypr::RealType *>(constant->type.get());      \
    ASSERT_EQ(realType->real, Vypr::Real::realType);                           \
    ASSERT_FALSE(realType->qualifiers.isConst);                                \
    ASSERT_FALSE(realType->isLValue);                                          \
  }

  PARSE_FLOAT_TEST(ZeroFloat, "0.0f", Float);
  PARSE_FLOAT_TEST(ZeroDouble, "0.0", Double);
  PARSE_FLOAT_TEST(ZeroLongDouble, "0.0l", Double);
  PARSE_FLOAT_TEST(CommonFloat, "20.40f", Float);
  PARSE_FLOAT_TEST(CommonDouble, "30.0", Double);
  PARSE_FLOAT_TEST(CommonLongDouble, "40.0l", Double);

#define PARSE_FLOAT_TEST(testName, content, realType)                          \
  TEST(Parse, testName)                                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L##content)); \
                                                                               \
    std::unique_ptr<Vypr::ConstantNode> constant =                             \
        Vypr::ConstantNode::Parse(lexer);                                      \
                                                                               \
    ASSERT_EQ(lexer.PeekToken().type, Vypr::CLangTokenType::NoToken);          \
    ASSERT_NE(constant, nullptr);                                              \
    ASSERT_EQ(constant->column, 1);                                            \
    ASSERT_EQ(constant->line, 1);                                              \
    ASSERT_EQ(constant->type->GetType(), Vypr::StorageMetaType::Real);         \
                                                                               \
    auto realType = dynamic_cast<Vypr::RealType *>(constant->type.get());      \
    ASSERT_EQ(realType->real, Vypr::Real::realType);                           \
    ASSERT_FALSE(realType->qualifiers.isConst);                                \
    ASSERT_FALSE(realType->isLValue);                                          \
  }

#define PARSE_CHARACTER_TEST(testName, content)                                \
  TEST(Parse, testName)                                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L##content)); \
                                                                               \
    std::unique_ptr<Vypr::ConstantNode> constant =                             \
        Vypr::ConstantNode::Parse(lexer);                                      \
                                                                               \
    ASSERT_EQ(lexer.PeekToken().type, Vypr::CLangTokenType::NoToken);          \
    ASSERT_NE(constant, nullptr);                                              \
    ASSERT_EQ(constant->column, 1);                                            \
    ASSERT_EQ(constant->line, 1);                                              \
    ASSERT_EQ(constant->type->GetType(), Vypr::StorageMetaType::Integral);     \
                                                                               \
    auto integralType =                                                        \
        dynamic_cast<Vypr::IntegralType *>(constant->type.get());              \
    ASSERT_EQ(integralType->integral, Vypr::Integral::Byte);                   \
    ASSERT_FALSE(integralType->qualifiers.isConst);                            \
    ASSERT_FALSE(integralType->isLValue);                                      \
    ASSERT_FALSE(integralType->isUnsigned);                                    \
  }

  PARSE_CHARACTER_TEST(CommonCharacter, "'b'");
  PARSE_CHARACTER_TEST(NullCharacter, "'\\0'");
  PARSE_CHARACTER_TEST(UnicodeCharacter, "'\\u1234'");

#define PARSE_STRING_TEST(testName, content)                                   \
  TEST(Parse, testName)                                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L##content)); \
                                                                               \
    std::unique_ptr<Vypr::ConstantNode> constant =                             \
        Vypr::ConstantNode::Parse(lexer);                                      \
                                                                               \
    ASSERT_EQ(lexer.PeekToken().type, Vypr::CLangTokenType::NoToken);          \
    ASSERT_NE(constant, nullptr);                                              \
    ASSERT_EQ(constant->column, 1);                                            \
    ASSERT_EQ(constant->line, 1);                                              \
    ASSERT_EQ(constant->type->GetType(), Vypr::StorageMetaType::Pointer);      \
                                                                               \
    auto pointerType =                                                         \
        dynamic_cast<Vypr::PointerType *>(constant->type.get());               \
    ASSERT_FALSE(pointerType->qualifiers.isConst);                             \
    ASSERT_FALSE(pointerType->isLValue);                                       \
  }

  PARSE_STRING_TEST(CommonString, "\"Hello, World\"");
  PARSE_STRING_TEST(EmptyString, "\"\"");

  TEST(GenerateCode, Int32Common)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"0"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 0);
  }

  TEST(GenerateCode, Int32Max)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"2147483647"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              std::numeric_limits<int32_t>::max());
  }

  TEST(GenerateCode, Int32AboveMaxAllowed)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"2147483648"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              2147483648LL);
  }

  TEST(GenerateCode, UInt32Common)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"120U"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 120U);
  }

  TEST(GenerateCode, UInt32Min)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"0U"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 0U);
  }

  TEST(GenerateCode, UInt32Max)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"4294967295U"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              std::numeric_limits<uint32_t>::max());
  }

  TEST(GenerateCode, UInt32AboveMaxAllowed)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"4294967296U"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              4294967296ULL);
  }

  TEST(GenerateCode, Int64Common)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"23LL"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 23LL);
  }

  TEST(GenerateCode, Int64Max)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"9223372036854775807LL"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              std::numeric_limits<int64_t>::max());
  }

  TEST(GenerateCode, Int64AboveMaxAllowed)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"9223372036854775808LL"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              9223372036854775808ULL);
  }

  TEST(GenerateCode, UInt64Common)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"23ULL"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 23ULL);
  }

  TEST(GenerateCode, UInt64Max)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"18446744073709551615ULL"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              std::numeric_limits<uint64_t>::max());
  }

  TEST(GenerateCode, UInt64AboveMax)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"18446744073709551616ULL"));
    ASSERT_THROW(Vypr::ConstantNode::Parse(lexer), Vypr::CompileError);
  }

  TEST(GenerateCode, UInt64Min)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"0ULL"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 64);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(),
              std::numeric_limits<uint64_t>::min());
  }

  TEST(GenerateCode, HexConstant)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"0x20"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 0x20);
  }

  TEST(GenerateCode, BinaryConstant)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"0b0110"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 0b0110);
  }

  TEST(GenerateCode, OctalConstant)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"023"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 32);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 023);
  }

  TEST(GenerateCode, CharacterConstant)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"'a'"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isIntegerTy());
    ASSERT_EQ(code->getType()->getIntegerBitWidth(), 8);
    ASSERT_EQ(llvm::dyn_cast<llvm::ConstantInt>(code)->getValue(), 'a');
  }

  TEST(GenerateCode, F32Common)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"2.0f"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isFloatTy());
    ASSERT_EQ(
        llvm::dyn_cast<llvm::ConstantFP>(code)->getValue().convertToFloat(),
        2.0f);
  }

  TEST(GenerateCode, F32NoDecimal)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"2f"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isFloatTy());
    ASSERT_EQ(
        llvm::dyn_cast<llvm::ConstantFP>(code)->getValue().convertToFloat(),
        2.0f);
  }

  TEST(GenerateCode, F32SciFormat)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"2e3f"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isFloatTy());
    ASSERT_EQ(
        llvm::dyn_cast<llvm::ConstantFP>(code)->getValue().convertToFloat(),
        2e3f);
  }

  TEST(GenerateCode, F64Common)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"2.0"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isDoubleTy());
    ASSERT_EQ(
        llvm::dyn_cast<llvm::ConstantFP>(code)->getValue().convertToDouble(),
        2.0);
  }

  TEST(GenerateCode, F64NoFraction)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"2."));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isDoubleTy());
    ASSERT_EQ(
        llvm::dyn_cast<llvm::ConstantFP>(code)->getValue().convertToDouble(),
        2.);
  }

  TEST(GenerateCode, F64Long)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"2.0L"));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isDoubleTy());
    ASSERT_EQ(
        llvm::dyn_cast<llvm::ConstantFP>(code)->getValue().convertToDouble(),
        2.0L);
  }

  TEST(GenerateCode, StringEmpty)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"\"\""));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isPointerTy());
    ASSERT_TRUE(llvm::cast<llvm::GlobalVariable>(code)
                    ->getInitializer()
                    ->isZeroValue());
  }

  TEST(GenerateCode, StringCommon)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"\"Hello, world\""));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isPointerTy());
    ASSERT_EQ(llvm::cast<llvm::ConstantDataArray>(
                  llvm::cast<llvm::GlobalVariable>(code)->getInitializer())
                  ->getAsCString()
                  .str(),
              "Hello, world");
  }

  TEST(GenerateCode, StringConcat)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"\"Hello,\"\n\" world\""));
    std::unique_ptr<Vypr::ConstantNode> constant =
        Vypr::ConstantNode::Parse(lexer);
    Vypr::Context context("TestModule");

    llvm::Value *code = constant->GenerateCode(context);

    ASSERT_TRUE(code->getType()->isPointerTy());
    ASSERT_EQ(llvm::cast<llvm::ConstantDataArray>(
                  llvm::cast<llvm::GlobalVariable>(code)->getInitializer())
                  ->getAsCString()
                  .str(),
              "Hello, world");
  }
} // namespace ConstantNodeTest