#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <loki/Threads.h>
#include <thread>

template <class Host>
class ClassLevelLockable
{
  inline static std::mutex mtx_;

public:
  class Lock;
  friend class Lock;

  class Lock
  {
    Lock(const Lock&);
    Lock& operator=(const Lock&);

  public:
    Lock() { mtx_.lock(); }
    explicit Lock(const ClassLevelLockable&) { mtx_.lock(); }
    ~Lock() { mtx_.unlock(); }
  };

  typedef volatile Host VolatileType;

  typedef int IntType;

  static IntType AtomicIncrement(volatile IntType& lval) { return __sync_fetch_and_add(&lval, 1); }
  static IntType AtomicDecrement(volatile IntType& lval) { return __sync_fetch_and_add(&lval, -1); }
  static void AtomicAssign(volatile IntType& lval, IntType val)
  {
    __sync_lock_test_and_set(&lval, val);
    __sync_synchronize();
  }
  static void AtomicAssign(IntType& lval, volatile IntType& val)
  {
    __sync_lock_test_and_set(&lval, val);
    __sync_synchronize();
  }
};

template <class Host>
class ObjectLevelLockable
{
  mutable std::mutex mtx_;

public:
  class Lock;
  friend class Lock;

  class Lock
  {
    ObjectLevelLockable const& host_;
    Lock(const Lock&);
    Lock& operator=(const Lock&);

  public:
    explicit Lock(const ObjectLevelLockable& host) : host_(host) { host_.mtx_.lock(); }
    ~Lock() { host_.mtx_.unlock(); }
  };

  typedef volatile Host VolatileType;

  typedef int IntType;

  static IntType AtomicIncrement(volatile IntType& lval) { return __sync_fetch_and_add(&lval, 1); }
  static IntType AtomicDecrement(volatile IntType& lval) { return __sync_fetch_and_add(&lval, -1); }
  static void AtomicAssign(volatile IntType& lval, IntType val)
  {
    __sync_lock_test_and_set(&lval, val);
    __sync_synchronize();
  }
  static void AtomicAssign(IntType& lval, volatile IntType& val)
  {
    __sync_lock_test_and_set(&lval, val);
    __sync_synchronize();
  }
};

class A
{
public:
  A() { ++constuctorCount; }
  std::string getName() const { return "A"; }
  inline static size_t constuctorCount;
};

// typedef Loki::SingletonHolder<A, Loki::CreateUsingNew, Loki::DefaultLifetime> SingleANew; fails thread sanitizer test
typedef Loki::SingletonHolder<A, Loki::CreateUsingNew, Loki::DefaultLifetime, ClassLevelLockable> SingleANew;

class TestClass
{
public:
  void operator()() { SingleANew::Instance(); }
};

TEST(Thread, TestThreadClassLevelLockableSingleton)
{
  std::thread t1(TestClass{});
  std::thread t2(TestClass{});
  t1.join();
  t2.join();
  EXPECT_EQ(A::constuctorCount, 1);
}