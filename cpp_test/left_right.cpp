#include <string>
#include <iostream>
class shape
{
};

void puts(const std::string& log)
{
    std::cout << log;
}
void foo(const shape &)
{
    puts("foo(const shape&)");
}

void foo(shape &&)
{
    puts("foo(shape&&)");
}

void bar(const shape &s)
{
    puts("bar(const shape&)");
    foo(s);
}

void bar(shape &&s)
{
    puts("bar(shape&&)");
    foo(s);
}

//与上面的两个bar是等价的，完美转发
template <typename T>
void bar(T &&s)
{
    foo(std::forward<T>(s));
}

int main()
{
    shape s;
    bar(s);
    //类型是右值引用的变量是一个左值
    bar(shape());

    foo(std::move(s));
}