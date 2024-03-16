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

TEST(SmartPtr, TestNoCopy)
{
  auto* ptr = new T{1, "test"};
  Loki::SmartPtr<T, Loki::NoCopy> tPtr1(ptr);
  tPtr1->i = 1;
  tPtr1->s = "test1";
  EXPECT_EQ(ptr->i, 1);
  EXPECT_EQ(ptr->s, "test1");
  EXPECT_EQ(tPtr1->i, 1);
  EXPECT_EQ(tPtr1->s, "test1");
}

TEST(SmartPtr, TestDeepCopy)
{
  auto* ptr = new TDeepCopy{1, "test"};
  Loki::SmartPtr<TDeepCopy, Loki::DeepCopy> tPtr1(ptr);
  Loki::SmartPtr<TDeepCopy, Loki::DeepCopy> tPtr2 = tPtr1;
  tPtr1->i = 1;
  tPtr1->s = "test1";
  tPtr2->i = 2;
  tPtr2->s = "test2";
  EXPECT_EQ(ptr->i, 1);
  EXPECT_EQ(ptr->s, "test1");
  EXPECT_EQ(tPtr1->i, 1);
  EXPECT_EQ(tPtr1->s, "test1");
  EXPECT_EQ(tPtr2->i, 2);
  EXPECT_EQ(tPtr2->s, "test2");
}
