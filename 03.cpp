// 03.cpp 明明的随机数

#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, temp;
    set<int> s1;

    cin >> n;
    while(n--) {
        cin >> temp;
        s1.insert(temp);
    }

    for(auto i : s1) {
        cout << i << endl;
    }
}

/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
int main()
{
    int n;
     
    int a;
    
    //以数组下标来存储随机数，下标对应的数组值为1，来说明是否是存储的随机数
    while(~scanf("%d", &n))
    {
        int count[1001] = {0};
		int i;
        for (i = 0; i < n; i++)
        {
            scanf("%d", &a);
            count[a] = 1;
        }
        for (i = 0; i < 1001; i++)
        {
            if (count[i] == 1)
            {
                printf("%d\n", i);
            }
        }
    }
     
    return 0;
}
*/