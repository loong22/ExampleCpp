// 05.cpp 进制转换

#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    scanf("%0x", &n);
    printf("%d\n", n);
    return 0;
}

/*
#include <iostream>
#include <unordered_map>
using namespace std;

int compute(int number, int isize) {
    int temp = 0;
    if(isize == 0)
    {
        return number;
    }else{
        temp = number;
        for(int i = 0; i < isize; ++i) {
            temp *= 16;
        }
    }
    return temp;
}

int main() {
    string str;
    cin >> str;

    unordered_map<char, int> m1 = {
        {'0', 0},
        {'1', 1},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'A', 10},
        {'B', 11},
        {'C', 12},
        {'D', 13},
        {'E', 14},
        {'F', 15}
    };

    int total = 0;
    if(str[0] == '0' && str[1] == 'x')
    {
        string newStr = str.substr(2,str.size());
        int isize = newStr.size();
        for(char ch : newStr)
        {
            isize--;
            total += compute(m1[ch], isize);
        }
    }
    cout << total << endl;
}
*/