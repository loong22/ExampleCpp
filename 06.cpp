// 06.cpp 质数因子

#include<iostream>
#include<cmath>
using namespace std;

int main(){
    long n;
    cin >> n;
    for(long i = 2; i <= sqrt(n) && i <= n; i++){  //从小到大的质因子，质因子至少有一个不会超过它的开方
        while(n % i == 0){ //所有的质数前面全部除掉，后续就不会有合因子
            cout << i << " ";
            n /= i; //除掉质因子
        }
    }
    if(n - 1) //自己本身就是质数
        cout << n << " ";
    return 0;
}
