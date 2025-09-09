#include "Vypr/AST/Type/PointerType.hpp"

#include <gtest/gtest.h>

#include "Vypr/AST/Type/ArrayType.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/RealType.hpp"

namespace PointerTypeTest
{
  TEST(PointerTypeTestConstruct, NotConstNotLValue)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, false);

    ASSERT_FALSE(type->qualifiers.isConst);
    ASSERT_FALSE(type->isLValue);
    ASSERT_EQ(type->GetType(), Vypr::StorageMetaType::Pointer);
  }

  TEST(PointerTypeTestConstruct, ConstNotLValue)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, true, false);

    ASSERT_TRUE(type->qualifiers.isConst);
    ASSERT_FALSE(type->isLValue);
    ASSERT_EQ(type->GetType(), Vypr::StorageMetaType::Pointer);
  }

  TEST(PointerTypeTestConstruct, NotConstLValue)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    ASSERT_FALSE(type->qualifiers.isConst);
    ASSERT_TRUE(type->isLValue);
    ASSERT_EQ(type->GetType(), Vypr::StorageMetaType::Pointer);
  }

  TEST(PointerTypeTestConstruct, ConstLValue)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, true, true);

    ASSERT_TRUE(type->qualifiers.isConst);
    ASSERT_TRUE(type->isLValue);
    ASSERT_EQ(type->GetType(), Vypr::StorageMetaType::Pointer);
  }

  TEST(PointerTypeTestClone, NotConstNotLValue)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other = type->Clone();

    ASSERT_NE(type.get(), other.get());
    ASSERT_EQ(type->qualifiers.isConst, other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(PointerTypeTestCheckPostfix, Increment)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::PostfixOp::Increment);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(PointerTypeTestCheckPostfix, Decrement)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::PostfixOp::Decrement);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(PointerTypeTestCheckUnary, Increment)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::Increment);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(PointerTypeTestCheckUnary, Decrement)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::Decrement);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

#define TEST_CHECK_INVALID_OP(testName, op)                                    \
  TEST(PointerTypeTestCheckUnary, testName)                                    \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
                                                                               \
    std::unique_ptr<Vypr::StorageType> other = type->Check(Vypr::op);          \
                                                                               \
    ASSERT_EQ(other, nullptr);                                                 \
  }

  TEST_CHECK_INVALID_OP(Negate, UnaryOp::Negate);
  TEST_CHECK_INVALID_OP(Not, UnaryOp::Not);

  TEST(PointerTypeTestCheckUnary, LogicalNot)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::LogicalNot);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(other.get())->integral,
              Vypr::Integral::Bool);
  }

  TEST(PointerTypeTestCheckUnary, DerefVoid)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::Deref);

    ASSERT_EQ(other, nullptr);
  }

  TEST(PointerTypeTestCheckUnary, Deref)
  {
    std::unique_ptr<Vypr::StorageType> storage =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, false,
                                             false);
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::Deref);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(other.get())->integral,
              Vypr::Integral::Int);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_TRUE(other->isLValue);
  }

  TEST(PointerTypeTestCheckUnary, AddressOf)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::AddressOf);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(PointerTypeTestCheckUnary, Sizeof)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);

    std::unique_ptr<Vypr::StorageType> other =
        type->Check(Vypr::UnaryOp::Sizeof);

    ASSERT_EQ(other->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(other.get())->integral,
              Vypr::Integral::Long);
    ASSERT_FALSE(other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

#define TEST_CHECK_BINARY_INVALID_OP_INT(op)                                   \
  TEST(PointerTypeTestCheckBinary, op##Int)                                    \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    auto other = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int,     \
                                                      false, false, false);    \
                                                                               \
    ASSERT_EQ(type->Check(Vypr::BinaryOp::op, *other), nullptr);               \
  }

#define TEST_CHECK_BINARY_INVALID_OP_REAL(op)                                  \
  TEST(PointerTypeTestCheckBinary, op##Real)                                   \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    auto other =                                                               \
        std::make_unique<Vypr::RealType>(Vypr::Real::Float, false, false);     \
                                                                               \
    ASSERT_EQ(type->Check(Vypr::BinaryOp::op, *other), nullptr);               \
  }

#define TEST_CHECK_BINARY_INVALID_OP_POINTER(op)                               \
  TEST(PointerTypeTestCheckBinary, op##Pointer)                                \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    auto other = type->Clone();                                                \
                                                                               \
    ASSERT_EQ(type->Check(Vypr::BinaryOp::op, *other), nullptr);               \
  }

#define TEST_CHECK_BINARY_INVALID_OP_ARRAY(op)                                 \
  TEST(PointerTypeTestCheckBinary, op##Array)                                  \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    storage = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, \
                                                   false, false);              \
    auto other = std::make_unique<Vypr::ArrayType>(storage, false, false);     \
                                                                               \
    ASSERT_EQ(type->Check(Vypr::BinaryOp::op, *other), nullptr);               \
  }

#define TEST_CHECK_BINARY_INVALID_OP_ALL(op)                                   \
  TEST_CHECK_BINARY_INVALID_OP_INT(op)                                         \
  TEST_CHECK_BINARY_INVALID_OP_REAL(op)                                        \
  TEST_CHECK_BINARY_INVALID_OP_POINTER(op)                                     \
  TEST_CHECK_BINARY_INVALID_OP_ARRAY(op)

  TEST_CHECK_BINARY_INVALID_OP_POINTER(Add);
  TEST_CHECK_BINARY_INVALID_OP_ARRAY(Add);
  TEST_CHECK_BINARY_INVALID_OP_REAL(Add);
  TEST_CHECK_BINARY_INVALID_OP_REAL(Subtract);
  TEST_CHECK_BINARY_INVALID_OP_ALL(Multiply);
  TEST_CHECK_BINARY_INVALID_OP_ALL(Divide);
  TEST_CHECK_BINARY_INVALID_OP_ALL(Modulo);
  TEST_CHECK_BINARY_INVALID_OP_ALL(ShiftLeft);
  TEST_CHECK_BINARY_INVALID_OP_ALL(ShiftRight);
  TEST_CHECK_BINARY_INVALID_OP_ALL(And);
  TEST_CHECK_BINARY_INVALID_OP_ALL(Xor);
  TEST_CHECK_BINARY_INVALID_OP_ALL(Or);
  TEST_CHECK_BINARY_INVALID_OP_INT(LessThan);
  TEST_CHECK_BINARY_INVALID_OP_REAL(LessThan);
  TEST_CHECK_BINARY_INVALID_OP_INT(LessEqual);
  TEST_CHECK_BINARY_INVALID_OP_REAL(LessEqual);
  TEST_CHECK_BINARY_INVALID_OP_INT(GreaterThan);
  TEST_CHECK_BINARY_INVALID_OP_REAL(GreaterThan);
  TEST_CHECK_BINARY_INVALID_OP_INT(GreaterEqual);
  TEST_CHECK_BINARY_INVALID_OP_REAL(GreaterEqual);
  TEST_CHECK_BINARY_INVALID_OP_INT(Equal);
  TEST_CHECK_BINARY_INVALID_OP_REAL(Equal);
  TEST_CHECK_BINARY_INVALID_OP_INT(NotEqual);
  TEST_CHECK_BINARY_INVALID_OP_REAL(NotEqual);

  TEST(PointerTypeTestCheckBinary, AddInt)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);
    auto other = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int,
                                                      false, false, false);

    auto result = type->Check(Vypr::BinaryOp::Add, *other);

    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(result->qualifiers.isConst);
    ASSERT_FALSE(result->isLValue);
  }

  TEST(PointerTypeTestCheckBinary, SubtractInt)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);
    auto other = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int,
                                                      false, false, false);

    auto result = type->Check(Vypr::BinaryOp::Add, *other);

    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_FALSE(result->qualifiers.isConst);
    ASSERT_FALSE(result->isLValue);
  }

  TEST(PointerTypeTestCheckBinary, SubtractPointer)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);
    storage = std::make_unique<Vypr::StorageType>();
    auto other = std::make_unique<Vypr::PointerType>(storage, false, true);

    auto result = type->Check(Vypr::BinaryOp::Subtract, *other);

    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,
              Vypr::Integral::Long);
    ASSERT_FALSE(result->qualifiers.isConst);
    ASSERT_FALSE(result->isLValue);
  }

  TEST(PointerTypeTestCheckBinary, SubtractArray)
  {
    auto storage = std::make_unique<Vypr::StorageType>();
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);
    storage = std::make_unique<Vypr::StorageType>();
    auto other = std::make_unique<Vypr::ArrayType>(storage, false, true);

    auto result = type->Check(Vypr::BinaryOp::Subtract, *other);

    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,
              Vypr::Integral::Long);
    ASSERT_FALSE(result->qualifiers.isConst);
    ASSERT_FALSE(result->isLValue);
  }

#define TEST_CHECK_BINARY_COMPARISON_POINTER(op)                               \
  TEST(PointerTypeTestCheckBinary, op##Pointer)                                \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    storage = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, \
                                                   false, false);              \
    auto other = std::make_unique<Vypr::PointerType>(storage, false, false);   \
                                                                               \
    auto result = type->Check(Vypr::BinaryOp::op, *other);                     \
                                                                               \
    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);             \
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,      \
              Vypr::Integral::Bool);                                           \
    ASSERT_FALSE(result->qualifiers.isConst);                                  \
    ASSERT_FALSE(result->isLValue);                                            \
  }

#define TEST_CHECK_BINARY_COMPARISON_INT(op)                                   \
  TEST(PointerTypeTestCheckBinary, op##Int)                                    \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    auto other = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int,     \
                                                      false, false, false);    \
                                                                               \
    auto result = type->Check(Vypr::BinaryOp::op, *other);                     \
                                                                               \
    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);             \
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,      \
              Vypr::Integral::Bool);                                           \
    ASSERT_FALSE(result->qualifiers.isConst);                                  \
    ASSERT_FALSE(result->isLValue);                                            \
  }

#define TEST_CHECK_BINARY_COMPARISON_REAL(op)                                  \
  TEST(PointerTypeTestCheckBinary, op##Real)                                   \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    auto other =                                                               \
        std::make_unique<Vypr::RealType>(Vypr::Real::Float, false, false);     \
                                                                               \
    auto result = type->Check(Vypr::BinaryOp::op, *other);                     \
                                                                               \
    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);             \
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,      \
              Vypr::Integral::Bool);                                           \
    ASSERT_FALSE(result->qualifiers.isConst);                                  \
    ASSERT_FALSE(result->isLValue);                                            \
  }

#define TEST_CHECK_BINARY_COMPARISON_POINTER(op)                               \
  TEST(PointerTypeTestCheckBinary, op##Pointer)                                \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    storage = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, \
                                                   false, false);              \
    auto other = std::make_unique<Vypr::PointerType>(storage, false, false);   \
                                                                               \
    auto result = type->Check(Vypr::BinaryOp::op, *other);                     \
                                                                               \
    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);             \
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,      \
              Vypr::Integral::Bool);                                           \
    ASSERT_FALSE(result->qualifiers.isConst);                                  \
    ASSERT_FALSE(result->isLValue);                                            \
  }

#define TEST_CHECK_BINARY_COMPARISON_ARRAY(op)                                 \
  TEST(PointerTypeTestCheckBinary, op##Array)                                  \
  {                                                                            \
    auto storage = std::make_unique<Vypr::StorageType>();                      \
    auto type = std::make_unique<Vypr::PointerType>(storage, false, true);     \
    storage = std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, \
                                                   false, false);              \
    auto other = std::make_unique<Vypr::ArrayType>(storage, false, false);     \
                                                                               \
    auto result = type->Check(Vypr::BinaryOp::op, *other);                     \
                                                                               \
    ASSERT_EQ(result->GetType(), Vypr::StorageMetaType::Integral);             \
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(result.get())->integral,      \
              Vypr::Integral::Bool);                                           \
    ASSERT_FALSE(result->qualifiers.isConst);                                  \
    ASSERT_FALSE(result->isLValue);                                            \
  }

#define TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(op)                         \
  TEST_CHECK_BINARY_COMPARISON_POINTER(op)                                     \
  TEST_CHECK_BINARY_COMPARISON_ARRAY(op)

#define TEST_CHECK_BINARY_COMPARISON_ALL(op)                                   \
  TEST_CHECK_BINARY_COMPARISON_INT(op)                                         \
  TEST_CHECK_BINARY_COMPARISON_REAL(op)                                        \
  TEST_CHECK_BINARY_COMPARISON_POINTER(op)                                     \
  TEST_CHECK_BINARY_COMPARISON_ARRAY(op)

  TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(LessThan);
  TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(LessEqual);
  TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(GreaterThan);
  TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(GreaterEqual);
  TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(Equal);
  TEST_CHECK_BINARY_COMPARISON_POINTER_ARRAY(NotEqual);

  TEST_CHECK_BINARY_COMPARISON_ALL(LogicalAnd);
  TEST_CHECK_BINARY_COMPARISON_ALL(LogicalOr);

} // namespace PointerTypeTest