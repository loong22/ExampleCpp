// 04.cpp 字符串分隔
#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str;
    while (cin >> str)
    {
        // 补0
        int len = str.size();
        if (len % 8 != 0)
        {
            int count = 8 - len % 8;
            str.append(count, '0');
        }

        // 按格式输出
        int newLen = str.size();
        for (int i = 0; i < newLen; i += 8)
        {
            cout << str.substr(i, 8) << endl;
        }
    }
    return 0;
}

/*
#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str;
    while (cin >> str)
    {
        int len = str.size();
        for (int i = 0; i < len; i += 8)
        {
            cout.width(8);
            cout.fill('0');
            cout << left << str.substr(i, 8) << endl;
        }
    }
    return 0;
}
*/

/*
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    getline(cin, str);

    int size = str.size() % 8;
    if (size != 0) {
        for(int i = 0; i < 8 - size; ++i)
        {
            str += "0";
        }
    }
    
    int temp = 0;
    for (char i : str) {
        std::cout << i;
        ++temp;
        if (temp % 8 == 0)
        {
            temp = 0;
            std::cout << std::endl;
        }
    }
}
*/