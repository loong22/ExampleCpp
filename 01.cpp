// 01.cpp 字符串最后一个单词的长度

#include <iostream>
using namespace std;

int main()
{
    string s;
    getline(cin, s);
    // rfind 成功查找返回索引（从 0 开始），否则返回 npos (size_t)(-1)
    cout << s.length() - 1 - s.rfind(' ') << endl;
}

/*
#include <iostream>
using namespace std;

int main() {
    string str;
    while (getline(cin, str)) {

        if (str.empty()) {
            cout << "0" << endl;
            continue;
        }

        int lastWordLength = 0;
        bool inWord = false;

        // 遍历字符串中的每个字符
        for (char ch : str) {
            if (ch == ' ' || ch == '\t') {  // 如果是空格或制表符，表示单词结束
                inWord = false;
            } else {
                if (!inWord) {  // 如果我们进入了一个新的单词
                    inWord = true;
                    lastWordLength = 1;  // 初始化单词长度为1
                } else {
                    lastWordLength++;  // 如果是在单词内部，增加单词长度
                }
            }
        }

        // 输出最后一个单词的长度
        cout << lastWordLength << endl;
    }
}
*/