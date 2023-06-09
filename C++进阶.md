
## 预处理

	#include”其实是非常“弱”的，不做什么检查，就是“死脑筋”把数据合并进源文件

	static uint32_t  calc_table[] = {
	#  include "calc_values.inc"        // 非常大的一个数组，细节被隐藏
	};

可以让 GCC 使用“-E”选项，略过后面的编译链接，只输出预处理后的源码

	g++ test03.cpp -E -o a.cxx    #输出预处理后的源码

写头文件的时候，为了防止代码被重复包含，通常要加上“Include Guard”，也就是用“#ifndef/#define/#endif”来保护整个头文件

	#ifndef _XXX_H_INCLUDED_
	#define _XXX_H_INCLUDED_
	
	...    // 头文件内容
	
	#endif // _XXX_H_INCLUDED_

宏定义前先检查，如果之前有定义就先 undef，然后再重新定义

	#ifdef AUTH_PWD                  // 检查是否已经有宏定义
	#  undef AUTH_PWD                // 取消宏定义
	#endif                           // 宏定义检查结束
	#define AUTH_PWD "xxx"           // 重新宏定义

_cplusplus”，它标记了 C++ 语言的版本号，使用它能够判断当前是 C 还是 C++，是 C++98 还是 C++11。
	
	#ifdef __cplusplus                      // 定义了这个宏就是在用C++编译
	    extern "C" {                        // 函数按照C的方式去处理
	#endif
	    void a_c_function(int a);
	#ifdef __cplusplus                      // 检查是否是C++编译
	    }                                   // extern "C" 结束
	#endif
	
	#if __cplusplus >= 201402                // 检查C++标准的版本号
	    cout << "c++14 or later" << endl;    // 201402就是C++14
	#elif __cplusplus >= 201103              // 检查C++标准的版本号
	    cout << "c++11 or before" << endl;   // 201103是C++11
	#else   // __cplusplus < 201103          // 199711是C++98
	#   error "c++ is too old"               // 太低则预处理报错
	#endif  // __cplusplus >= 201402         // 预处理语句结束

使用“#if 1”“#if 0”来显式启用或者禁用大段代码，要比“/* … */”的注释方式安全得多，也清楚得多


	#if 0          // 0即禁用下面的代码，1则是启用
	  ...          // 任意的代码
	#endif         // 预处理结束
	
	#if 1          // 1启用代码，用来强调下面代码的必要性
	  ...          // 任意的代码
	#endif         // 预处理结束


## 编译
“属性”没有新增关键字，而是用两对方括号的形式“[[…]]”，方括号的中间就是属性标签，在 C++11 里只定义了两个属性：“noreturn”和“carries_dependency”，C++14 的情况略微好了点，增加了一个比较实用的属性“deprecated”，用来标记不推荐使用的变量、函数或者类，也就是被“废弃”。

	[[noreturn]]              // 属性标签
	int func(bool flag)       // 函数绝不会返回任何值
	{
	    throw std::runtime_error("XXX");
	}


	[[deprecated("deadline:2020-12-31")]]      // C++14 or later
	int old_func();

好在“属性”也支持非标准扩展，允许以类似名字空间的方式使用编译器自己的一些“非官方”属性，比如，GCC 的属性都在“gnu::”里。

* deprecated：与 C++14 相同，但可以用在 C++11 里。
* unused：用于变量、类型、函数等，表示虽然暂时不用，但最好保留着，因为将来可能会用。
* constructor：函数会在 main() 函数之前执行，效果有点像是全局对象的构造函数。
* destructor：函数会在 main() 函数结束之后执行，有点像是全局对象的析构函数。
* always_inline：要求编译器强制内联函数，作用比 inline 关键字更强。
* hot：标记“热点”函数，要求编译器更积极地优化。

### 断言
assert 虽然是一个宏，但在预处理阶段不生效，而是在运行阶段才起作用，所以又叫“动态断言”。有了“动态断言”

那么相应的也就有“静态断言”，名字也很像，叫“static_assert”，不过它是一个专门的关键字，而不是宏。因为它只在编译时生效，运行阶段看不见，所以是“静态”的。

## 异常
总结了几个应当使用异常的判断准则：

* 不允许被忽略的错误；
* 极少数情况下才会发生的错误；
* 严重影响正常流程，很难恢复到正常状态的错误；
* 无法本地处理，必须“穿透”调用栈，传递到上层才能被处理的错误。

规则听起来可能有点不好理解，我给你举几个例子。比如说构造函数，如果内部初始化失败，无法创建，那后面的逻辑也就进行不下去了，所以这里就可以用异常来处理。

再比如，读写文件，通常文件系统很少会出错，总会成功，如果用错误码来处理不存在、权限错误等，就显得太啰嗦，这时也应该使用异常。

相反的例子就是 socket 通信。因为网络链路的不稳定因素太多，收发数据失败简直是“家常便饭”。虽然出错的后果很严重，但它出现的频率太高了，使用异常会增加很多的处理成本，为了性能考虑，还是检查错误码重试比较好。

建议自定义异常处理

	class my_exception : public std::runtime_error
	{
	public:
	    using this_type     = my_exception;        // 给自己起个别名
	    using super_type    = std::runtime_error;  // 给父类也起个别名
	public:
	    my_exception(const char* msg):            // 构造函数
	        super_type(msg)                      // 别名也可以用于构造
	    {}  
	
	    my_exception() = default;                // 默认构造函数
	   ~my_exception() = default;                // 默认析构函数
	private:
	    int code = 0;                            // 其他的内部私有数据
	};


	[[noreturn]]                      // 属性标签
	void raise(const char* msg)      // 函数封装throw，没有返回值
	{
	    throw my_exception(msg);     // 抛出异常，也可以有更多的逻辑
	}


### function-try

	void f(int i)
	try
	{
	   if ( i  < 0 )
	      throw"less than zero";
	   std::cout <<"greater than zero" << std::endl;
	}
	catch(const char* e)
	{
	    std::cout << e << std::endl;
	}
	
	int main() {
	        f(1);
	        f(-1);
	        return 0;
	}

### 异常安全
异常安全是指当异常发生时，既不会发生资源泄漏，系统也不会处于一个不一致的状态。


	#include <iostream>
	#include <stdexcept>
	
	using namespace std;
	
	class matrix
	{
	public:
	  friend matrix
	  operator*(const matrix&,
	            const matrix&);
	    matrix(size_t nrows, size_t ncols) : nrows_(nrows), ncols_(ncols)
	    {
	        data_ = new float[nrows * ncols];
	    }
	    ~matrix()
	    {
	        delete[] data_;
	    }
	
	private:
	    float *data_;
	    size_t nrows_;
	    size_t ncols_;
	};
	
	matrix operator*(const matrix& lhs,
	                 const matrix& rhs)
	{
	  if (lhs.ncols_ != rhs.nrows_) {
	    throw std::runtime_error(
	      "matrix sizes mismatch");
	  }
	  matrix result(lhs.nrows_, rhs.ncols_);
	  // 进行矩阵乘法运算
	  return result;
	}
	
	int main()
	{
	    matrix a(5, 5);
	    matrix b(5, 5);
	    matrix c = a * b;
	}

使用虽然没有catch异常，但是仍然是异常安全的，看看可能会出现错误 / 异常的地方：首先是内存分配。
* 如果 new 出错，按照 C++ 的规则，一般会得到异常 bad_alloc，对象的构造也就失败了。这种情况下，在 catch 捕捉到这个异常之前，所有的栈上对象会全部被析构，资源全部被自动清理。
* 如果是矩阵的长宽不合适不能做乘法呢？我们同样会得到一个异常，这样，在使用乘法的地方，对象 c 根本不会被构造出来。
* 如果在乘法函数里内存分配失败呢？一样，result 对象根本没有构造出来，也就没有 c 对象了。还是一切正常。
* 如果 a、b 是本地变量，然后乘法失败了呢？析构函数会自动释放其空间，我们同样不会有任何资源泄漏。


## 处理文本
string 其实并不是一个“真正的类型”，而是模板类 basic_string 的特化形式，是一个 typedef：

	using string = std::basic_string<char>;  // string其实是一个类型别名
把每个字符串都看作是一个不可变的实体，你才能在 C++ 里真正地用好字符串。需要存储字符的容器，比如字节序列、数据缓冲区，建议最好改用vector<char>

### 字面量后缀
C++14 为方便使用字符串，新增了一个字面量的后缀“s”，明确地表示它是 string 字符串类型，而不是 C 字符串，这就可以利用 auto 来自动类型推导，而且在其他用到字符串的地方，也可以省去声明临时字符串变量的麻烦，效率也会更高


	using namespace std::literals::string_literals;  //必须打开名字空间
	
	auto str = "std string"s;      // 后缀s，表示是标准字符串，直接类型推导
	
	assert("time"s.size() == 4);   // 标准字符串可以直接调用成员函数

### 原始字符串
C++11 还为字面量增加了一个“原始字符串”（Raw string literal）的新表示形式，比原来的引号多了一个大写字母 R 和一对圆括号

	auto str = R"(nier:automata)";    // 原始字符串：nier:automata
