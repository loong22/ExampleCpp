// 09.cpp 字符个数统计

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int main() {
    string str;
    getline(cin, str);

    unordered_map<int, int> m1;
    for(auto it : str) {
        int temp = it;
        if(temp >= 33 && temp <=126) {
            m1[temp] = 1;
        }
    }
    cout << m1.size()  << endl;
}
// 64 位输出请用 printf("%lld")

/*
#include<iostream>
#include<set>
using namespace std;
int main() {
    string text;    //存放输入的字符串
    getline(cin, text);    //获取输入的一行字符串
    set<char> s;    //使用set容器
    for(int i=0;i<text.length();i++) {
        s.insert(text[i]);    //将text中的字符逐个添加到set容器中
    }
    cout<<s.size()<<endl;    //输出set容器的长度，即为不同字符的个数
}
*/