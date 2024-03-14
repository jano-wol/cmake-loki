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

int linear(int a, int b, int x) { return a * x + b; }

void printer1(std::stringstream& ss) { ss << "printer1 message. "; }

void printer2(std::stringstream& ss) { ss << "printer2 message. "; }

TEST(Functor, TestCreation)
{
  Loki::Functor<std::string, TYPELIST_1(int)> cmd1(Function);
  SomeFunctor fn;
  Loki::Functor<std::string, TYPELIST_1(int)> cmd2(fn);
  SomeClass myObject;
  Loki::Functor<std::string, TYPELIST_1(int)> cmd3(&myObject, &SomeClass::MemberFunction);
  Loki::Functor<std::string, TYPELIST_1(int)> cmd4(cmd3);

  EXPECT_EQ(cmd1(1), "Function=1");
  EXPECT_EQ(cmd2(-1), "SomeFunctor=-1");
  EXPECT_EQ(cmd3(-1), "MemberFunction=-1");
  EXPECT_EQ(cmd4(1), "MemberFunction=1");
}

TEST(Functor, TestBind)
{
  Loki::Functor<int, TYPELIST_3(int, int, int)> cmd1(linear);
  Loki::Functor<int, TYPELIST_2(int, int)> cmd2(BindFirst(cmd1, 10));
  EXPECT_EQ(cmd1(10, 5, 7), 75);
  EXPECT_EQ(cmd2(5, 7), 75);
}

TEST(Functor, TestChain)
{
  Loki::Functor<void, TYPELIST_1(std::stringstream&)> cmd1(printer1);
  Loki::Functor<void, TYPELIST_1(std::stringstream&)> cmd2(printer2);
  Loki::Functor<void, TYPELIST_1(std::stringstream&)> cmd3(Chain(cmd1, cmd2));
  std::stringstream ss;
  cmd3(ss);
  EXPECT_EQ(ss.str(), "printer1 message. printer2 message. ");
}