/* 
 * File:   D.cpp
 * Author: Kimi
 *
 * Created on 2009年9月27日, 上午1:10
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

struct Tpoint
{
    int x,y,r;
} P[10];

/*
 *
 */

double dist(int a, int b)
{
    return sqrt((P[a].x-P[b].x)*(P[a].x-P[b].x)+(P[a].y-P[b].y)*(P[a].y-P[b].y));
}

int main() {
    freopen("D-small-attempt0.in","r",stdin);
    freopen("D-small.out","w",stdout);
    int tt;
    scanf("%d",&tt);
    for (int t=0; t<tt; t++) {
        int n;
        scanf("%d",&n);
        for (int i=0; i<n; i++) scanf("%d%d%d",&P[i].x,&P[i].y,&P[i].r);
        if (n==1) {
            printf("Case #%d: %.8lf\n",t+1,double(P[0].r));
            continue;
        }
        if (n==2) {
            printf("Case #%d: %.8lf\n",t+1,double(max(P[0].r,P[1].r)));
            continue;
        }
        if (n==3) {
            double ans=20000;
            ans=min(ans,max(double(P[0].r),(P[1].r+P[2].r+dist(1,2))*.5));
            ans=min(ans,max(double(P[1].r),(P[0].r+P[2].r+dist(0,2))*.5));
            ans=min(ans,max(double(P[2].r),(P[0].r+P[1].r+dist(0,1))*.5));
            printf("Case #%d: %.8lf\n",t+1,ans);
            continue;
        }
    }
    return (EXIT_SUCCESS);
}
