#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <set>
#include <map>
#include <list>
#include <limits>
#include <queue>
#include <stdexcept>
#include <iomanip> 
#include <sstream>
long long dfs(int p);
using namespace std;
#define RR 50
#define CC 50
char str[RR][CC];
//#define TRY
#define SMALL
//#define LARGE
long long  Solve(int n,int m,int k);

int main() 
{
#ifdef TRY
	freopen("1.txt", "rt", stdin);
	freopen("2.out","wt",stdout);
#endif
#ifdef SMALL
	freopen("C-small-attempt0.in","rt",stdin);
	freopen("C-small.out","wt",stdout);
#endif
#ifdef LARGE
	freopen("A-large.in","rt",stdin);
	freopen("A-large.out","wt",stdout);
#endif
	int Numcase;
	cin>>Numcase;
	char c;
	int n,m,k;
	//*
	string temp;
	for(int test=1;test<=Numcase;test++)
	{
		cout<<"Case #"<<test<<": ";

		cin>>n>>m>>k;
		cout<<Solve(n,m,k)<<endl;
	}
	//*/
}
long long  Solve(int n,int m,int k)
{
	if(n==1||n==2||m==1||m==2)
		return k;
	int temp;
	if(n>m)
	{
		temp=n;
		n=m;
		m=temp;
	}//n<=m
	if(n==3 && m==3)
	{
		if(k>=5)
			return k-1;
		else
			return k;
	}
	if(n==3 && m==4)
	{
		if(k>=8)
			return k-2;
		else if(k==7)
			return 6;
		else if(k==6)
			return 5;
		else if(k==5)
			return 4;
		else
			return k;
	}
	if(n==3 && m==5)
	{
		if(k<=4)
			return k;
		else if(k>=5 && k<=7)
			return k-1;
		else if(k>=8 && k<=10)
			return k-2;
		else if(k>=11)
			return k-3;
	}
	if(n==3 && m==6)
	{
		if(k<=4)
			return k;
		else if(k>=5 && k<=7)
			return k-1;
		else if(k>=8 && k<=10)
			return k-2;
		else if(k>=11 && k<=13)
			return k-3;
		else if(k>=14)
			return k-4;
	}
	if(n==4 && m==4)
	{
		if(k<=4)
			return k;
		else if(k>=5 && k<=7)
			return k-1;
		else if(k>=8 && k<=9)
			return k-2;
		else if(k>=10 && k<=11)
			return k-3;
		else if(k>=12)
			return k-4;
	}
	if(n==4 && m==5)
	{
		if(k<=4)
			return k;
		else if(k>=5 && k<=7)
			return k-1;
		else if(k>=8 && k<=9)
			return k-2;
		else if(k>=10 && k<=11) //11 8
			return k-3;
		else if(k>=12 && k<=13)
			return k-4;
		else if(k<=15 && k>=14)
			return k-5;
		else if(k>=16)
			return k-6;
	}
}