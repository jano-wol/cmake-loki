#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <iostream>
#include <sstream>

std::stringstream collector;

class A
{
public:
  ~A() { collector << "a"; }
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
  ~E()
  {
    collector << "e";
    if (collector.str() != "bdcae") {
      exit(1);
    }
  }
};

typedef Loki::SingletonHolder<A, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleA;
typedef Loki::SingletonHolder<B, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleB;
typedef Loki::SingletonHolder<C, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleC;
typedef Loki::SingletonHolder<D, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleD;
typedef Loki::SingletonHolder<E, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleE;

// Singletons with larger longevity will be destructed first. This contradicts the Modern C++ book's statement.
// This contradiction is known: http://erdani.org/index.php/books/modern-c-design/errata/index.html
// See the corrections of p. 140
inline unsigned GetLongevity(A*) { return 5; }
inline unsigned GetLongevity(B*) { return 8; }
inline unsigned GetLongevity(C*) { return 5; }
inline unsigned GetLongevity(D*) { return 5; }
inline unsigned GetLongevity(E*) { return 1; }

TEST(Singleton, TestLongevity)
{
  SingleA::Instance();
  SingleB::Instance();
  SingleC::Instance();
  SingleD::Instance();
  SingleE::Instance();
}
