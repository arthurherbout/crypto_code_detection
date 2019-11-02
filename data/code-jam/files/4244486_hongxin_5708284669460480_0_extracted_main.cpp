
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<string>
#include<cmath>
#include<vector>
#include<queue>
#include<set>
#include<stack>
#include<map>
#include<algorithm>
using namespace std;
#define maxN 10
#define INF 0X7F7F7F7F

int K,L,S;
char keys[maxN];
char target[maxN];
char str[maxN];
int pi[maxN];
double maxNum;
double sumNum;

int calc_prefix(char* s,int* pi)
{
    int k,q;
    pi[1] = 0;
    k = 0;
    for(q = 2;q <= L;q++)
    {
        while(k != 0&&s[k+1] != s[q])
        {
            k = pi[k];
        }
        if(s[k+1] == s[q])
        {
            k++;
        }
        pi[q] = k;
    }
    return 0;
}

int calcNum() {
    int q = 0;
    int num = 0;
    for(int i = 1;i <= S;i++) {
        while(q != 0 && target[q+1] != str[i]) {
            q = pi[q];
        }
        if(target[q+1] == str[i]) {
            q++;
        }
        if(q == L)  {
            num++;
            q = pi[q];
        }
    }
    return num;
}

int DFS(int strLen) {
    if(strLen == S+1) {
        str[strLen] = 0;
        int tmpNum = calcNum();
        sumNum += tmpNum;
        maxNum = max(maxNum, (double)tmpNum);
        return 0;
    }
    for(int i = 1;i <= K;i++) {
        str[strLen] = keys[i];
        DFS(strLen+1);
    }
    return 0;
}

int solve() {
    double ans;
    sumNum = 0;
    maxNum = -1;
    calc_prefix(target, pi);
    DFS(1);
    ans = maxNum-sumNum/pow((double)K, (double)S);
    printf("%.7llf\n", ans);
    return 0;
}

int main() {
    freopen("B-small-attempt0.in","r",stdin);
    freopen("out1.txt","w",stdout);
    int T;
    scanf("%d",&T);
    for(int t = 1;t <= T;t++) {

        cout<<"Case #"<<t<<": ";
        cin>>K>>L>>S;
        getchar();
        memset(keys, 0, sizeof(keys));
        memset(target, 0, sizeof(target));
        gets(keys+1);
        gets(target+1);
        solve();
    }
    return 0;
}


