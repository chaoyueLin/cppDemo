每个 lambda 表达式都有一个全局唯一的类型


举一个例子，如果你写：

	auto adder = [n](int x) { return n + x; };

编译器产生的代码类似于：

	struct lambda_1266ab7e {
	  lambda_1266ab7e(int n) : n_{n} {}
	  auto operator()(int x) const
	  {
	    return n_ + x;
	  }
	private:
	  int n_;
	};
	
	auto adder = lambda_1266ab7e(n);

一个 lambda 表达式除了没有名字之外，还有一个特点是你可以立即进行求值。这就使得我们可以把一段独立的代码封装起来，达到更干净、表意的效果。

	[](int x) { return x * x; }(3)

这个表达式的结果是 3 的平方 9。即使这个看似无聊的例子，都是有意义的，因为它免去了我们定义一个 constexpr 函数的必要。只要能满足 constexpr 函数的条件，一个 lambda 表达式默认就是 constexpr 函数。

另外一种用途是解决多重初始化路径的问题。


	Obj obj;
	switch (init_mode) {
	case init_mode1:
	  obj = Obj(…);
	  break;
	case init_mode2;
	  obj = Obj(…);
	  break;
	…
	}

更直截了当的做法是用一个 lambda 表达式来进行改造，既可以提升性能（不需要默认函数或拷贝 / 移动），又让初始化部分显得更清晰：


	auto obj = [init_mode]() {
	  switch (init_mode) {
	  case init_mode1:
	    return Obj(…);
	    break;
	  case init_mode2:
	    return Obj(…);
	    break;
	  …
	  }
	}();



变量捕获现在我们来细看一下 lambda 表达式中变量捕获的细节。

* 本地变量名标明对其按值捕获
* & 加本地变量名标明对其按引用捕获（
* this 标明按引用捕获外围对象（针对 lambda 表达式定义出现在一个非静态类成员内的情况）；注意默认捕获符 = 和 & 号可以自动捕获 this（并且在 C++20 之前，在 = 后写 this 会导致出错）
* *this 标明按值捕获外围对象（针对 lambda 表达式定义出现在一个非静态类成员内的情况；C++17 新增语法）
* 变量名 = 表达式 标明按值捕获表达式的结果（可理解为 auto 变量名 = 表达式）
* &变量名 = 表达式 标明按引用捕获表达式的结果（可理解为 auto& 变量名 = 表达式）


一般而言，按值捕获是比较安全的做法。按引用捕获时则需要更小心些，必须能够确保被捕获的变量和 lambda 表达式的生命期至少一样长，并在有下面需求之一时才使用：
* 需要在 lambda 表达式中修改这个变量并让外部观察到
* 需要看到这个变量在外部被修改的结果
* 这个变量的复制代价比较高


按引用捕获：

	vector<int> v1;
	vector<int> v2;

	auto push_data = [&](int n) {
	  // 或使用 [&v1, &v2] 捕捉
	  v1.push_back(n);
	  v2.push_back(n)
	};
	
	push_data(2);
	push_data(3);


按值捕获外围对象

	
	#include <chrono>
	#include <iostream>
	#include <sstream>
	#include <string>
	#include <thread>
	
	using namespace std;
	
	int get_count()
	{
	  static int count = 0;
	  return ++count;
	}
	
	class task {
	public:
	  task(int data) : data_(data) {}
	  auto lazy_launch()
	  {
	    return
	      [*this, count = get_count()]()
	      mutable {
	        ostringstream oss;
	        oss << "Done work " << data_
	            << " (No. " << count
	            << ") in thread "
	            << this_thread::get_id()
	            << '\n';
	        msg_ = oss.str();
	        calculate();
	      };
	  }
	  void calculate()
	  {
	    this_thread::sleep_for(100ms);
	    cout << msg_;
	  }
	
	private:
	  int data_;
	  string msg_;
	};
	
	int main()
	{
	  auto t = task{37};
	  thread t1{t.lazy_launch()};
	  thread t2{t.lazy_launch()};
	  t1.join();
	  t2.join();
	}


