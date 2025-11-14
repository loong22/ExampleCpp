// 02.cpp 计算某字符出现次数

#include <iostream>

using namespace std;

int main()
{
    string s;
    getline(cin, s);

    char c = ' ';
    cin >> c;

    if(int(c) >= 65 && int(c) < 97)
        c = tolower(c);

    unsigned int n = 0;
    for (auto i : s) {
        if (tolower(i) == c) {
            ++n;
        }
    }
    cout << n << endl;
}

/*
#include<iostream>
#include<string>

using namespace std;

int main()
{
    string str;
    char s;
    int count = 0;
    getline(cin,str);
    cin>>s;
    if(s>64)//判断对比字符是否为字母
    {
        for(int i=0;i<str.size();i++)
        {
            if(str[i] == s || str[i]+32 == s || str[i]-32==s)
            {
                count++;
            }
        }
    }
    else
    {
        for(int j=0;j<str.size();j++)
        {
            if(str[j]==s)
            {
                count++;
            }
        }
    }
    
    cout << count << endl;
    return 0;
}

*/