/* 
 * File:   C.cpp
 * Author: Kimi
 *
 * Created on 2009年9月27日, 上午1:26
 */

#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <complex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <unistd.h>
#define Fill(A,n) memset(A,n,sizeof(A))
#define pb push_back

using namespace std;

struct Tprice
{
    int p[50];
} price[20];

int n,k,best,dp,last[20];
bool f[50][50];
/*
 *
 */

bool cmp(Tprice a, Tprice b)
{
    return a.p[0]<b.p[0];
}

bool touch(int a, int b)
{
    for (int i=0; i<k; i++)
        if (price[a].p[i]>=price[b].p[i]) return true;
    return false;
}

void dfs(int dep)
{
    if (dep==n) {
        best=min(best,dp);
        return;
    }
    bool been=false;
    for (int i=0; i<dp; i++)
        if (!f[dep][last[i]]) {
            been=true;
            int temp=last[i];
            last[i]=dep;
            dfs(dep+1);
            last[i]=temp;
        }
    if (!been) {
        last[dp++]=dep;
        dfs(dep+1);
        dp--;
    }
}

int main() {
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small.out","w",stdout);
    int tt;
    scanf("%d",&tt);
    for (int t=0; t<tt; t++) {
        scanf("%d%d",&n,&k);
        for (int i=0; i<n; i++) {
            for (int j=0; j<k; j++)
                scanf("%d",&price[i].p[j]);
        }
        sort(price,price+n,cmp);
        Fill(f,false);
        for (int i=0; i<n; i++) {
            for (int j=i+1; j<n; j++)
                if (touch(i,j)) f[i][j]=f[j][i]=true;
        }
        dp=0;
        best=10000;
        dfs(0);
        printf("Case #%d: %d\n",t+1,best);
    }
    return (EXIT_SUCCESS);
}
