#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <iostream>

class A
{
public:
  void print() const { std::cout << "A\n"; }
};

typedef Loki::SingletonHolder<A, Loki::CreateUsingNew> SingleA;

TEST(Singleton, TestSingleton)
{
  const auto& a = SingleA::Instance();
  a.print();
}
