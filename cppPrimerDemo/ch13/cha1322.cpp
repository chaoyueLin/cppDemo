#include <string>
#include<iostream>
using namespace std;


class HashPtr
{
private:
    std::string *ps;
    int i;

public:
    HashPtr(const std::string &s = std::string()) : ps(new std::string(s)), i(0) {}
    HashPtr(const HashPtr &hp) : ps(new std::string(*hp.ps)), i(hp.i) {}
    HashPtr &operator=(const HashPtr &hp)
    {
        auto new_p = new std::string(*hp.ps);
        delete ps;
        ps = new_p;
        i = hp.i;
        return *this;
    }
    ~HashPtr(){
        delete ps;
    }
};


//这里是演示没有定义拷贝构造函数，将会拷贝指针的值，指向同一个地址。
class test{
	public:
	string* data;
};

 
int main(){
	//测试对象赋值 
	test t1;
	string temp="hi";
	t1.data=&temp;
	test t2;
	t2=t1;
	cout<<"t1.data="<<t1.data<<" t2.data="<<t2.data<<endl;
    
    *(t1.data)="11";
	cout<<"t1.data="<<*(t1.data)<<"t2.data="<<*(t2.data)<<endl;
	
	*(t2.data)="22";
	cout<<"t1.data="<<*(t1.data)<<"t2.data="<<*(t2.data)<<endl;
	
}
