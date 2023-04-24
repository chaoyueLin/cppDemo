
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>

using namespace std;

class scoped_thread {
public:
  template <typename... Arg>
  scoped_thread(Arg&&... arg)
    : thread_(
        std::forward<Arg>(arg)...)
  {}
  scoped_thread(
    scoped_thread&& other)
    : thread_(
        std::move(other.thread_))
  {}
  scoped_thread(
    const scoped_thread&) = delete;
  ~scoped_thread()
  {
    if (thread_.joinable()) {
      thread_.join();
    }
  }

private:
  thread thread_;
};

void work(condition_variable& cv,
          int& result)
{
  // 假装我们计算了很久
  this_thread::sleep_for(2s);
  result = 42;
  cv.notify_one();
}

int main()
{
  condition_variable cv;
  mutex cv_mut;
  int result;

  scoped_thread th{work, ref(cv),
                   ref(result)};
  // 干一些其他事
  cout << "I am waiting now\n";
  unique_lock lock{cv_mut};
  cv.wait(lock);
  cout << "Answer: " << result
       << '\n';
}