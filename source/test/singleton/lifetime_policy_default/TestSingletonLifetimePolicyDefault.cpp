#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <iostream>

class Log
{
public:
  ~Log() { std::cout << "log dies\n"; }
};
inline unsigned GetLongevity(Log*) { return 8; }
typedef Loki::SingletonHolder<Log, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleLog;

class A
{
public:
  ~A() { std::cout << "a dies\n"; }
};
inline unsigned GetLongevity(A*) { return 5; }
typedef Loki::SingletonHolder<A, Loki::CreateUsingNew, Loki::SingletonWithLongevity> SingleA;

TEST(SingletonLifetimePolicyDefault, TestSingletonLifetimePolicyDefault)
{
  SingleA::Instance();
  SingleLog::Instance();
}
