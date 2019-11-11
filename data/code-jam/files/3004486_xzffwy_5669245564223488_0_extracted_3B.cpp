/////////////////////////////////////////////////////////////////////////
// File Name: 3B.cpp
// Author: wang
// mail: 1062239606@qq.com
// Created Time: 2014/5/11 18:14:54
/////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <cmath>

#include <algorithm>
#include<iostream>
#include<queue>
#include <map>
using namespace std;
typedef long long ll;
#define INF (INT_MAX/10)
#define SQR(x) ((x)*(x))
#define rep(i, n) for (int i=0; i<(n); ++i)
#define repf(i, a, b) for (int i=(a); i<=(b); ++i)
#define repd(i, a, b) for (int i=(a); i>=(b); --i)
#define clr(ar,val) memset(ar, val, sizeof(ar))
#define N  100
char s[N+10][N+10];
char p[N*N+20];
bool flag[N+10];
bool sign[30];
int ans[N+10];
int Max=0;
int m;
void dfs(int t)
{
	if(t==m)
	{
		memset(sign,false,sizeof(sign));
		int j=0;
		rep(i,m){
           int k=0;
		   while(s[ans[i]][k]!='\0') p[j++]=s[ans[i]][k++];
		}
		int fl=0;
		sign[p[0]-'a']=true;
		repf(i,1,j-1)
			if(p[i]!=p[i-1])
              if(sign[p[i]-'a']==false)
				  sign[p[i]-'a']=true;
			  else fl=1;
		if(fl==0) Max++;
	}
	rep(i,m)
		if(flag[i]==false)
		{
			flag[i]=true;
			ans[t]=i;
			dfs(t+1);
			flag[i]=false;
		}
}
int main(){
	int n;
	freopen("e:/acm/pro/in","r",stdin);
	freopen("e:/acm/pro/out.out","w",stdout);
	scanf("%d",&n);
	repf(ror,1,n){
		scanf("%d",&m);
		rep(i,m)
		{
			scanf("%s",s[i]);
		}
		memset(flag,false,sizeof(flag));
		Max=0;
		rep(i,m){
			if(flag[i]==false)
			{
				ans[0]=i;
				flag[i]=true;
				dfs(1);
				flag[i]=false;
			}
		}
		printf("Case #%d: %d\n",ror,Max);
	}
}

