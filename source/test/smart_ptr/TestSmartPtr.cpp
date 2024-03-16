#include <gmock/gmock.h>

#include <loki/SmartPtr.h>
#include <iostream>
#include <sstream>

struct T
{
  int i;
  std::string s;
};

struct TDeepCopy
{
  int i;
  std::string s;

  TDeepCopy* Clone() { return new TDeepCopy{i, s}; }
};

TEST(SmartPtr, TestSmartPtr)
{
  Loki::SmartPtr<T> tPtr1 = new T{1, "test"};
  Loki::SmartPtr<T> tPtr2 = tPtr1;
  tPtr1->i = 1;
  tPtr1->s = "test1";
  tPtr2->i = 2;
  tPtr2->s = "test2";
  EXPECT_EQ(tPtr1->i, 2);
  EXPECT_EQ(tPtr1->s, "test2");
  EXPECT_EQ(tPtr2->i, 2);
  EXPECT_EQ(tPtr2->s, "test2");
}

TEST(SmartPtr, TestDeepCopy)
{
  Loki::SmartPtr<TDeepCopy, Loki::DeepCopy> tPtr1 = new TDeepCopy{1, "test"};
  Loki::SmartPtr<TDeepCopy, Loki::DeepCopy> tPtr2 = tPtr1;
  tPtr1->i = 1;
  tPtr1->s = "test1";
  tPtr2->i = 2;
  tPtr2->s = "test2";
  EXPECT_EQ(tPtr1->i, 1);
  EXPECT_EQ(tPtr1->s, "test1");
  EXPECT_EQ(tPtr2->i, 2);
  EXPECT_EQ(tPtr2->s, "test2");
}

TEST(SmartPtr, TestNoCopy)
{
  Loki::SmartPtr<T, Loki::NoCopy> tPtr1 = new T{1, "test"};
  // Loki::SmartPtr<T, Loki::NoCopy> tPtr2 = tPtr1;
  tPtr1->i = 1;
  tPtr1->s = "test1";
  EXPECT_EQ(tPtr1->i, 1);
  EXPECT_EQ(tPtr1->s, "test1");
}

TEST(SmartPtr, TestDestructiveCopy)
{
  typedef Loki::SmartPtr<T, Loki::DestructiveCopy, Loki::DisallowConversion, Loki::RejectNull> DestructivePtr;
  DestructivePtr tPtr1 = new T{1, "test"};
  DestructivePtr tPtr2 = tPtr1;
  tPtr2->i = 2;
  tPtr2->s = "test2";
  EXPECT_EQ(tPtr2->i, 2);
  EXPECT_EQ(tPtr2->s, "test2");
  EXPECT_ANY_THROW(tPtr1->i);
}
