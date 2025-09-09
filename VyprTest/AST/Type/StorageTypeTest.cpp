#include "Vypr/AST/Type/StorageType.hpp"

#include <gtest/gtest.h>

namespace StorageTypeTest
{
  TEST(Construct, Void)
  {
    std::unique_ptr<Vypr::StorageType> voidType =
        std::make_unique<Vypr::StorageType>();

    ASSERT_EQ(voidType->GetType(), Vypr::StorageMetaType::Void);
    ASSERT_FALSE(voidType->qualifiers.isConst);
    ASSERT_FALSE(voidType->isLValue);
  }

  TEST(Clone, Void)
  {
    std::unique_ptr<Vypr::StorageType> voidType =
        std::make_unique<Vypr::StorageType>();

    std::unique_ptr<Vypr::StorageType> otherType = voidType->Clone();

    ASSERT_NE(voidType, otherType);
    ASSERT_EQ(voidType->GetType(), otherType->GetType());
    ASSERT_EQ(voidType->qualifiers.isConst, otherType->qualifiers.isConst);
    ASSERT_EQ(voidType->isLValue, otherType->isLValue);
  }
} // namespace StorageTypeTest