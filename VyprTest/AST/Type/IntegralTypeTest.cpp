#include "Vypr/AST/Type/IntegralType.hpp"

#include <gtest/gtest.h>

namespace IntegralTypeTest
{
  TEST(IntegralTypeTestConstruct, NotConstNotLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             false);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->qualifiers.isConst, false);
    ASSERT_EQ(type->isLValue, false);
  }

  TEST(IntegralTypeTestConstruct, ConstNotLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, true,
                                             false);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->qualifiers.isConst, true);
    ASSERT_EQ(type->isLValue, false);
  }

  TEST(IntegralTypeTestConstruct, NotConstLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             true);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->qualifiers.isConst, false);
    ASSERT_EQ(type->isLValue, true);
  }

  TEST(IntegralTypeTestConstruct, ConstLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             true);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->qualifiers.isConst, false);
    ASSERT_EQ(type->isLValue, true);
  }

  TEST(IntegralTypeTestConstruct, OtherIntegralType)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);

    ASSERT_EQ(type->integral, Vypr::Integral::Long);
    ASSERT_EQ(type->qualifiers.isConst, false);
    ASSERT_EQ(type->isLValue, true);
  }

  TEST(IntegralTypeTestClone, NotConstNotLValue)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> other = type->Clone();

    ASSERT_NE(type.get(), other.get());
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(type.get())->integral,
              dynamic_cast<Vypr::IntegralType *>(other.get())->integral);
    ASSERT_EQ(type->qualifiers.isConst, other->qualifiers.isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(IntegralTypeTestCheckArithmetic, Void)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>();

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, *other);

    ASSERT_EQ(result, nullptr);
  }

  TEST(IntegralTypeTestCheckArithmetic, IntegralLowerPriority)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, *other);
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->qualifiers.isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_FALSE(resultIntegral->isUnsigned);
  }

  TEST(IntegralTypeTestCheckArithmetic, IntegralHigherPriority)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, *other);
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->qualifiers.isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_FALSE(resultIntegral->isUnsigned);
  }

  TEST(IntegralTypeTestCheckArithmetic, IntegralEqualPriority)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, *other);
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Int);
    ASSERT_FALSE(resultIntegral->qualifiers.isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_FALSE(resultIntegral->isUnsigned);
  }

  TEST(IntegralTypeTestCheckArithmetic, IntegralLowerPriorityUnsigned)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, true, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, *other);
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->qualifiers.isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_TRUE(resultIntegral->isUnsigned);
  }

  TEST(IntegralTypeTestCheckArithmetic, IntegralHigherPriorityUnsigned)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, true, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, *other);
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->qualifiers.isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_TRUE(resultIntegral->isUnsigned);
  }

  TEST(IntegralTypeTestCheckBitwise, Real)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>(Vypr::StorageMetaType::Real, false,
                                            false);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::ShiftLeft, *other);

    ASSERT_EQ(result, nullptr);
  }

  TEST(IntegralTypeTestCheckBitwise, Pointer)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>(Vypr::StorageMetaType::Pointer,
                                            false, false);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::ShiftLeft, *other);

    ASSERT_EQ(result, nullptr);
  }

  TEST(IntegralTypeTestCheckBitwise, Array)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>(Vypr::StorageMetaType::Array, false,
                                            false);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::ShiftLeft, *other);

    ASSERT_EQ(result, nullptr);
  }
} // namespace IntegralTypeTest