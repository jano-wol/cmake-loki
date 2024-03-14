#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <iostream>

class A
{
public:
  A() { ++constuctorCount; }
  std::string getName() const { return "A"; }
  static size_t constuctorCount;
};

size_t A::constuctorCount = 0;

TEST(Singleton, TestSingletonCreationPolicy)
{
  typedef Loki::SingletonHolder<A, Loki::CreateUsingNew> SingleANew;
  typedef Loki::SingletonHolder<A, Loki::CreateUsingMalloc> SingleAMalloc;
  typedef Loki::SingletonHolder<A, Loki::CreateStatic> SingleAStatic;

  EXPECT_EQ(A::constuctorCount, 0);
  SingleANew::Instance();
  EXPECT_EQ(A::constuctorCount, 1);
  SingleAMalloc::Instance();
  EXPECT_EQ(A::constuctorCount, 2);
  SingleAStatic::Instance();
  EXPECT_EQ(A::constuctorCount, 3);
  const auto& aNew = SingleANew::Instance();
  const auto& aMalloc = SingleAMalloc::Instance();
  const auto& aStatic = SingleAStatic::Instance();
  EXPECT_EQ(A::constuctorCount, 3);
  EXPECT_EQ(aNew.getName(), "A");
  EXPECT_EQ(aMalloc.getName(), "A");
  EXPECT_EQ(aStatic.getName(), "A");
  EXPECT_NE(aNew.getName(), "B");
  EXPECT_NE(aMalloc.getName(), "B");
  EXPECT_NE(aStatic.getName(), "B");
}
