//safe sub
#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<vector>
#include<list>
#include<queue>
#include<map>
#include<utility>
#include<set>
#include<algorithm>
#include<string>
#include<iomanip>
#include <cassert>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
#include<unordered_set>
#include<unordered_map>
#include <stack> 
#define boost  std::ios::sync_with_stdio(false)//;cin.tie(0)
#define F first
#define S second
#define tr(container, it)  for(__typeof(container.begin()) it = container.begin(); it != container.end(); it++)
#define trr(container, it)  for(__typeof(container.rbegin()) it = container.rbegin(); it != container.rend(); it++)
#define pb push_back
#define in insert
#define mp make_pair
#define fi(var,a,b) for(int var=a;var<=b;var++)
#define fe(var,a,b) for(int var=a;var<b;var++)
#define cin(x) scanf("%d",&x)
#define cout(x) printf("%d",x)
#define newl printf("\n")
#define news printf(" ")
#define inf 2000000000
#define MOD 1000000007
#define endl '\n'
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> pi;
typedef pair<string,int> psi;
int main()
{
freopen("input.txt","r",stdin);
freopen("outputwali2.txt","w",stdout);
int test;cin>>test;
fi(tt,1,test)
{
int r,c;cin>>r>>c;int rr=r;int cc=c;
char arr[r][c];
vi mark(26,0);
fe(i,0,r)
{
fe(j,0,c)
{
cin>>arr[i][j];
}
}

fe(i,0,r)
{
fe(j,0,c)
{
if(arr[i][j]!='?'&&mark[arr[i][j]-'A']==0)
{
mark[arr[i][j]-'A']=1;
char cval=arr[i][j];
int lft=j,rht=j;
while((arr[i][lft-1]=='?'||arr[i][lft-1]==cval)&&lft-1>=0)
{--lft;arr[i][lft]=cval;}
while((arr[i][rht+1]=='?'||arr[i][rht+1]==cval)&&rht+1<c)
{++rht;arr[i][rht]=cval;}

int rrr=i;
while(true)
{
if(rrr-1<0)
break;
int flag=0;
fi(x,lft,rht)
{
if(arr[rrr-1][x]!='?')
flag=1;
}
if(flag==0)
{
fi(x,lft,rht)
{
arr[rrr-1][x]=cval;
}
}
else
break;
rrr-=1;
}
rrr=i;
while(true)
{
if(rrr+1>=r)
break;
int flag=0;
fi(x,lft,rht)
{
if(arr[rrr+1][x]!='?')
flag=1;
}
if(flag==0)
{
fi(x,lft,rht)
{
arr[rrr+1][x]=cval;
}
}
else
break;
rrr+=1;
}
}
}
}


cout<<"Case #"<<tt<<":"<<endl;
fe(i,0,r)
{
fe(j,0,c)
{
cout<<arr[i][j];
}
cout<<endl;
}

}
}



















