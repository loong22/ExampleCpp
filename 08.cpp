// 08.cpp 合并表记录

#include <map>
#include <iostream>
using namespace std;

int main() {
    int a;
    cin >> a;
    int key, value;
    map<int, int> num;
    while (a--) { 
        cin >> key >> value;
        num[key] += value;
    }

    for (auto it : num)
    {
        cout << it.first << " " << it.second << endl;
    }
}