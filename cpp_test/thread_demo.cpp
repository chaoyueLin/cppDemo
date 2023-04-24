#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

void thread_one()
{
    puts("hello");
}

void thread_two(int num, string& str)
{
    cout << "num:" << num << ",name:" << str << endl;
}
void f1(int n)
{
    for (int i = 0; i < 5; ++i)
    {
	cout << "=====Thread:" << n << "======" << endl;
	this_thread::sleep_for(chrono::microseconds(10));
    }
}

void f2(int& n)
{
    for (int i = 0; i < 5; ++i)
    {
	cout << "thread two executing" << endl;
	++n;
	this_thread::sleep_for(chrono::microseconds(10));
    }
}
int main(int argc, char* argv[])
{
    thread tt(thread_one);
    tt.join();
    string str = "luckin";
    thread yy(thread_two, 88, ref(str));   //这里要注意是以引用的方式调用参数
    yy.detach();

    int n = 0;
    thread t1;    //这是一个空的线程对象，还不是一个线程
    thread t2(f1, n + 1);
    thread t3(f2, ref(n));
    thread t4(move(t3));    //t3不再是一个线程
    t2.join();
    t4.join();
    cout << "the result n is:" << n << endl;
    system("pause");
    return 0;
}