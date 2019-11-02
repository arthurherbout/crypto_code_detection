#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

#define Rep(i,n) for(int i=0;i<(n);++i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define pb push_back
#define MP make_pair

typedef pair<int,int> PII;
typedef vector<int> VI;

int r;
bool a[111][111],b[111][111];

int main() {
	int st;
	cin>>st;
	Rep(tt,st){
		cerr<<tt+1<<"/"<<st<<endl;
		cin>>r;
		memset(a,false,sizeof(a));
		Rep(i,r){
			int x1,y1,x2,y2;
			cin>>x1>>y1>>x2>>y2;
			For(x,x1,x2)For(y,y1,y2)a[x][y]=true;	
		}
		int time=0;
		while(true){
			bool tiep=false;
			Rep(i,111)Rep(j,111)if(a[i][j])tiep=true;
			if(!tiep)break;
			++time;
			memmove(b,a,sizeof(a));
			Rep(i,111)Rep(j,111){
				if((i==0||!a[i-1][j])&&(j==0||!a[i][j-1]))b[i][j]=false;
				if(i>0&&j+1<111&&a[i][j]&&a[i-1][j+1])b[i][j+1]=true;
			}
			memmove(a,b,sizeof(a));
		}
		cout<<"Case #"<<tt+1<<": ";
		cout<<time<<endl;
	}
	return 0;	
}
