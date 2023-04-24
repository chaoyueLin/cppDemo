
#include <chrono>
#include <future>
#include <iostream>
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

void work(promise<int> prom)
{
  // 假装我们计算了很久
  this_thread::sleep_for(2s);
  prom.set_value(42);
}

int main()
{
  promise<int> prom;
  auto fut = prom.get_future();
  scoped_thread th{work,
                   move(prom)};
  // 干一些其他事
  cout << "I am waiting now\n";
  cout << "Answer: " << fut.get()
       << '\n';
}