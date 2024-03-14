#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <iostream>
#include <sstream>

std::stringstream collector;

class A
{
public:
  ~A()
  {
    collector << "a";
    if (collector.str() != "edcba") {
      exit(1);
    }
  }
};

class B
{
public:
  ~B() { collector << "b"; }
};

class C
{
public:
  ~C() { collector << "c"; }
};

class D
{
public:
  ~D() { collector << "d"; }
};

class E
{
public:
  ~E() { collector << "e"; }
};

typedef Loki::SingletonHolder<A> SingleA;
typedef Loki::SingletonHolder<B, Loki::CreateUsingNew, Loki::PhoenixSingleton> SingleB;
typedef Loki::SingletonHolder<C> SingleC;
typedef Loki::SingletonHolder<D, Loki::CreateUsingNew, Loki::PhoenixSingleton> SingleD;
typedef Loki::SingletonHolder<E> SingleE;

TEST(Singleton, TestPhoenix)
{
  SingleA::Instance();
  SingleB::Instance();
  SingleC::Instance();
  SingleD::Instance();
  SingleE::Instance();
}

class Log
{
public:
  std::string getName() { return "name"; };
};
typedef Loki::SingletonHolder<Log, Loki::CreateUsingNew, Loki::PhoenixSingleton> SingleLog;

class F
{
public:
  ~F()
  {
    auto n = SingleLog::Instance().getName();
    if (n != "name") {
      exit(1);
    }
  }
};
typedef Loki::SingletonHolder<F> SingleF;

TEST(Singleton, TestNoThrow)
{
  SingleF::Instance();
  SingleLog::Instance();
}
