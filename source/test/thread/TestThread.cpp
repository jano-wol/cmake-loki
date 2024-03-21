#include <gmock/gmock.h>

#include <loki/Singleton.h>
#include <loki/Threads.h>
#include <time.h>
#include <chrono>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

class TestClass
{
public:
  void operator()(int i, int j)
  {
    // std::this_thread::sleep_for(std::chrono::milliseconds(rand()%1000));
    const std::lock_guard<std::mutex> lock(mx);
    map_[i] = j;
  }
  inline static std::map<int, int> map_;
  inline static std::mutex mx;
};

TEST(Thread, TestThread)
{
  srand((unsigned)time(NULL));
  std::thread t1(TestClass(), 0, 1);
  std::thread t2(TestClass(), 1, 2);
  t1.join();
  t2.join();
  EXPECT_EQ(TestClass::map_[0], 1);
}