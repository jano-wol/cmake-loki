#include <gmock/gmock.h>

#include <loki/Functor.h>
#include <iostream>
#include <sstream>

std::string Function(int a)
{
  std::stringstream ss;
  ss << "Function=" << a;
  return ss.str();
}

struct SomeFunctor
{
  std::string operator()(int a)
  {
    std::stringstream ss;
    ss << "SomeFunctor=" << a;
    return ss.str();
  }
};

struct SomeClass
{
  std::string MemberFunction(int a)
  {
    std::stringstream ss;
    ss << "MemberFunction=" << a;
    return ss.str();
  }
};

TEST(Functor, TestCreation)
{
  Loki::Functor<std::string, TYPELIST_1(int)> cmd1(Function);
  SomeFunctor fn;
  Loki::Functor<std::string, TYPELIST_1(int)> cmd2(fn);
  SomeClass myObject;
  Loki::Functor<std::string, TYPELIST_1(int)> cmd3(&myObject, &SomeClass::MemberFunction);
  Loki::Functor<std::string, TYPELIST_1(int)> cmd4(cmd3);

  EXPECT_EQ(cmd1(1), "Function=1");
  EXPECT_EQ(cmd1(1.1), "Function=1");
  EXPECT_EQ(cmd2(-1), "SomeFunctor=-1");
  EXPECT_EQ(cmd2(-1.1), "SomeFunctor=-1");
  EXPECT_EQ(cmd3(-1), "MemberFunction=-1");
  EXPECT_EQ(cmd3(-1.9), "MemberFunction=-1");
  EXPECT_EQ(cmd4(1), "MemberFunction=1");
  EXPECT_EQ(cmd4(1.1), "MemberFunction=1");
}
