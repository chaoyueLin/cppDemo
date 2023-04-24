#include<stdio.h>
#include <iostream>


using namespace std;

union Data{
    bool val1;
    char val2;
    int val3;
    long val4;
};

int main(){
    Data data;
    data.val3=300;
    
    cout << "sizeof(data) " << sizeof(data) << endl;
    cout << "sizeof(data.val1) " << sizeof(data.val1) << endl;
    cout << "data.val2=" << data.val2 << endl;
    cout << "data.val1=" << data.val1 << endl;
    cout << "data.val3=" << data.val3 << endl;
    cout << "data.val4=" << data.val4 << endl;
    
}