
#include "output_container.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
using namespace std;

int main()
{
  list<int> lst{1, 7, 2, 8, 3};
  vector<int> vec{1, 7, 2, 8, 3};

  sort(vec.begin(), vec.end());    // 正常
  // sort(lst.begin(), lst.end()); // 会出错
  lst.sort();                      // 正常

  cout << lst << endl;
  // 输出 { 1, 2, 3, 7, 8 }

  cout << vec << endl;
  // 输出 { 1, 2, 3, 7, 8 }
}