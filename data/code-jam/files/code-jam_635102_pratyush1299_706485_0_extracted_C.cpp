/*
ID: your_id_here
PROG: test
LANG: C++
*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
//BEGIN_TEMPLATE_BY_PRATYUSH_VERMA
#define INF 1<<31
#define REP(i,a) for((i)=0;(i)<(a);(i)++)
#define FOR(i,a,b) for((i)=(a);(i)<(b);(i)++)
#define FOREACH(i,c) for(__typeof((c).begin()) (i)=(c).begin();(i)!=(c).end();(i)++)
#define ALL(x) (x).begin(),(x).end()
#define SIZE(x) ((int)(x.size()))
#define LENGTH(x) ((int)(x.length()))
#define MP(x,y) make_pair(x,y)
//END_TEMPLATE_BY_PRATYUSH_VERMA
using namespace std;
bool all0(bool arr[][105])
{
	int i,j;
	for(i=1;i<=100;i++)
	for(j=1;j<=100;j++)
	if(arr[i][j]==1)
	return true;
	return false;
}
int main()
{
ios_base::sync_with_stdio(0);
freopen("C.in" , "r" , stdin);
freopen("C.txt" , "w" , stdout);
int test,no;
cin>>test;
for(no=1;no<=test;no++)
{
	int r,x1,x2,y1,y2,x,y,xx,yy,i,j;
	cin>>r;
	bool arr[105][105]={false};
	bool prev[105][105]={false};
 	bool lock[105][105]={false};
//	memset(arr,false,sizeof(arr));
	
	for(i=1;i<=100;i++)
	for(j=1;j<=100;j++)
	arr[i][j]=0;
    int count=0;
	for(int xxx=0;xxx<r;xxx++)
	{
		cin>>x1>>y1>>x2>>y2;
		x=min(x1,x2);
		y=min(y1,y2);
		xx=max(x1,x2);
		yy=max(y1,y2);
		for(i=y;i<=yy;i++)
		{
			for(j=x;j<=xx;j++)
			{
				arr[i][j]=1;
			}
		}
	}
/*	for(i=1;i<=6;i++)
	{
		for(j=1;j<=6;j++)
		cout<<arr[i][j];
		cout<<endl;
	}
	cout<<endl;
*/	while(all0(arr))
	{
//	for(int p=1;p<=6;p++)
	memcpy(prev,arr,sizeof(arr));
/*	for(i=1;i<=6;i++)
	{
			for(j=1;j<=6;j++)
			cout<<prev[i][j];
			cout<<endl;
	}
	memset(lock,false,sizeof(lock));
	*/for(i=1;i<=100;i++)
	{
		for(j=1;j<=100;j++)
		{
			if((i==1 || j==1) && arr[i][j]==1)
			{
				arr[i][j]=0;
//				if(i-1!=0) lock[i-1][j]=true;
//				if(j-1!=0) lock[i][j-1]=true;
//				lock[i][j]=1;
			}
			else if(arr[i][j]==1 && prev[i-1][j]==0 && prev[i][j-1]==0) //
			{
				arr[i][j]=0;
//				lock[i-1][j]=0;
//				lock[i][j-1]=0;
//				lock[i][j]=1;
			}
			else if(arr[i][j]==0 && prev[i-1][j]==1 && prev[i][j-1]==1)
			{
				arr[i][j]=1;
//				lock[i][j-1]=0;
//				lock[i-1][j]=0;
//				lock[i][j]=1;
			}
		}
	}
//	cout<<endl<<count<<" : "<<endl;
	count++;
//	getchar();
	}
	cout<<"Case #"<<no<<": "<<count<<endl;
}
return 0;
}
