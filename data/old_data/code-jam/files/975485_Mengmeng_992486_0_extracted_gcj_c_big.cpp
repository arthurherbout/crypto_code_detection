/* 
 * File:   gcj_c.cpp
 * Author: lmm333
 *
 * Created on 2011年5月7日, 上午8:54
 */
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <cstring>
#include <cmath>
using namespace std;
#define PI acos(double(-1))
#define put printf
#define get scanf
#define geti(x) scanf("%d",&x)
#define getf(x) scanf("%lf",&x)
#define mem(x) memset(x,0,sizeof(x))

#define maxx(x,y) ((x) > (y) ? (x) : (y))
#define minn(x,y) ((x) <= (y) ? (x) : (y))

const int INF = 2000000000;
typedef long long llong;

int comp(int a, int b) {
    return a > b;
}
int x[1010];

int main() {
    freopen("C-large.in","r",stdin);
    freopen("out2.txt","w",stdout);
    int a, b, cas, n, sum, yihuo,ans,min;
    scanf("%d", &cas);
    for (int j = 1; j <= cas; ++j) {
        sum = 0;
        yihuo = 0;
        min=INF;
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", x + i);
            sum += x[i];
            yihuo = yihuo^x[i];
            if(x[i]<min)
                min=x[i];
        }
        if (yihuo != 0) {
            printf("Case #%d: NO\n", j);
        } else {
            printf("Case #%d: %d\n",j,sum-min);
        }
    }
    //system("pause");
    return 0;
}
