#include "core/slot.h"

#include <gtest/gtest.h>

TEST(SlotTest, Slot_ShouldBeCopyable) {
    using SlotType = tajga::core::Slot<void (int)>;

    EXPECT_TRUE(std::is_default_constructible<SlotType>::value);
    EXPECT_TRUE(std::is_copy_constructible<SlotType>::value);
    EXPECT_TRUE(std::is_copy_assignable<SlotType>::value);
    EXPECT_TRUE(std::is_destructible<SlotType>::value);
}

TEST(SlotTest, SlotShouldBeInitiallyEmpty) {
  auto slot = tajga::core::Slot<void (int)>{};

  EXPECT_TRUE(slot.IsEmpty());
}

TEST(SlotTest, CallingEmptySlotShouldAssert) {
  auto slot = tajga::core::Slot<void (int)>{};

  ASSERT_TRUE(slot.IsEmpty());

  ASSERT_DEATH(slot(42), "");
}

class StaticFunctionSlotTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    call_count = 0;
  }

  static void Function(int n) {
    ++call_count;
  }

  static void AnotherFunction(int n) {
  }

  static int call_count;

}; // class StaticFunctionSlotTest

int StaticFunctionSlotTest::call_count = 0;

TEST_F(StaticFunctionSlotTest, StaticFunctionShouldBeProperlyCalled) {
  EXPECT_EQ(0, call_count);

  auto slot = tajga::core::Slot<void (int)>{&Function};

  ASSERT_FALSE(slot.IsEmpty());

  slot(42);

  EXPECT_EQ(1, call_count);

  slot(666);
  slot(1);

  EXPECT_EQ(3, call_count);
}

TEST_F(StaticFunctionSlotTest, CopyCtorShouldCreateCopyWithTheSameDelegate) {
  EXPECT_EQ(0, call_count);

  auto slot1 = tajga::core::Slot<void (int)>{&Function};
  auto slot2 = tajga::core::Slot<void (int)>{slot1};

  ASSERT_FALSE(slot2.IsEmpty());
  EXPECT_EQ(slot1, slot2);

  slot1(42);
  EXPECT_EQ(1, call_count);

  slot2(666);
  slot2(1);

  EXPECT_EQ(3, call_count);
}

TEST_F(StaticFunctionSlotTest, CopyingEmptySlotShouldCreateAnotherEmptySlot) {
  auto slot1 = tajga::core::Slot<void (int)>{};
  ASSERT_TRUE(slot1.IsEmpty());

  auto slot2 = tajga::core::Slot<void (int)>{slot1};
  ASSERT_TRUE(slot2.IsEmpty());
}

/*TEST_F(StaticFunctionSlotTest, CopySlotShouldNotCopyConnection) {
  EXPECT_EQ(0, call_count);

  auto signal1 = tajga::Signal<void (int)>{};
  auto signal2 = tajga::Signal<void (int)>{};

  auto slot1 = tajga::core::Slot<void (int)>{&Function};
  Connect(&signal1, &slot1);
  Connect(&signal2, &slot1);

  EXPECT_TRUE(slot1.HasAnyConnections());

  auto slot2(slot1);

  EXPECT_TRUE(slot1.HasAnyConnections());
  EXPECT_FALSE(slot2.HasAnyConnections());
}*/

TEST_F(StaticFunctionSlotTest, BindShouldCreateSlotWithGivenFunction) {
  EXPECT_EQ(0, call_count);

  auto slot = tajga::core::Slot<void (int)>::Bind(&Function);

  slot(42);
  EXPECT_EQ(1, call_count);
}

TEST_F(StaticFunctionSlotTest, SlotsBoundToTheSameFunctionShouldBeEqual) {
  auto slot1 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot2 = tajga::core::Slot<void (int)>::Bind(&Function);

  EXPECT_EQ(slot1, slot2);
}

TEST_F(StaticFunctionSlotTest, SlotsBoundToDifferentFunctionsShouldNotBeEqual) {
  auto slot1 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot2 = tajga::core::Slot<void (int)>::Bind(&AnotherFunction);

  EXPECT_NE(slot1, slot2);
}

class MemberFunctionSlotTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    call_count = 0;
  }

  struct FunctorHelper {
    void Function(int n) {
      ++call_count;
    }
    void AnotherFunction(int n) {
    }
  }; // struct FunctorHelper

  FunctorHelper helper;
  static int call_count;

}; // class MemberFunctionSlotTest

int MemberFunctionSlotTest::call_count = 0;

TEST_F(MemberFunctionSlotTest, MemberFunctionShouldBeProperlyCalled) {
  EXPECT_EQ(0, call_count);

  auto slot = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::Function};

  ASSERT_FALSE(slot.IsEmpty());

  slot(42);

  EXPECT_EQ(1, call_count);

  slot(666);
  slot(1);

  EXPECT_EQ(3, call_count);
}

TEST_F(MemberFunctionSlotTest, BindShouldCreateSlotWithGivenFunction) {
  EXPECT_EQ(0, call_count);

  auto slot = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::Function};

  slot(42);
  EXPECT_EQ(1, call_count);
}

TEST_F(MemberFunctionSlotTest, SlotsBoundToTheSameMethodShouldBeEqual) {
  auto slot1 = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::Function};
  auto slot2 = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::Function};

  EXPECT_EQ(slot1, slot2);
}

TEST_F(MemberFunctionSlotTest, SlotsBoundToDifferentFunctionsOfTheSameInstanceShouldNotBeEqual) {
  auto slot1 = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::Function};
  auto slot2 = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::AnotherFunction};

  EXPECT_NE(slot1, slot2);
}

TEST_F(MemberFunctionSlotTest, SlotsBoundToDifferentInstancesAndTheSameFunctionsShouldNotBeEqual) {
  auto slot1 = tajga::core::Slot<void (int)>{&helper, &FunctorHelper::Function};

  FunctorHelper another_helper;
  auto slot2 = tajga::core::Slot<void (int)>{&another_helper, &FunctorHelper::Function};

  EXPECT_NE(slot1, slot2);
}

TEST_F(StaticFunctionSlotTest, OperatorLT) {
  auto slot1 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot2 = tajga::core::Slot<void (int)>::Bind(&AnotherFunction);

  EXPECT_NE(slot1, slot2);
  EXPECT_TRUE(slot1 < slot2 || slot2 < slot1);
}

TEST_F(StaticFunctionSlotTest, OperatorGT) {
  auto slot1 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot2 = tajga::core::Slot<void (int)>::Bind(&AnotherFunction);

  EXPECT_NE(slot1, slot2);
  EXPECT_TRUE(slot1 > slot2 || slot2 > slot1);
}

TEST_F(StaticFunctionSlotTest, OperatorLE) {
  auto slot1 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot2 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot3 = tajga::core::Slot<void (int)>::Bind(&AnotherFunction);

  EXPECT_EQ(slot1, slot2);
  EXPECT_LE(slot1, slot2);
  EXPECT_LE(slot2, slot1);

  EXPECT_NE(slot1, slot3);
  EXPECT_TRUE(slot1 <= slot3 || slot3 <= slot1);
}

TEST_F(StaticFunctionSlotTest, OperatorGE) {
  auto slot1 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot2 = tajga::core::Slot<void (int)>::Bind(&Function);
  auto slot3 = tajga::core::Slot<void (int)>::Bind(&AnotherFunction);

  EXPECT_EQ(slot1, slot2);
  EXPECT_GE(slot1, slot2);
  EXPECT_GE(slot2, slot1);

  EXPECT_NE(slot1, slot3);
  EXPECT_TRUE(slot1 >= slot3 || slot3 >= slot1);
}

