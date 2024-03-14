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
typedef Loki::SingletonHolder<B> SingleB;
typedef Loki::SingletonHolder<C> SingleC;
typedef Loki::SingletonHolder<D> SingleD;
typedef Loki::SingletonHolder<E> SingleE;

TEST(Singleton, TestDefaultLifetime)
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
typedef Loki::SingletonHolder<Log> SingleLog;

class F
{
public:
  ~F() { EXPECT_ANY_THROW(SingleLog::Instance().getName()); }
};
typedef Loki::SingletonHolder<F> SingleF;

TEST(Singleton, TestThrow)
{
  SingleF::Instance();
  SingleLog::Instance();
}
