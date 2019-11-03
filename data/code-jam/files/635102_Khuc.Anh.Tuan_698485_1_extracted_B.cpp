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

int M[1024];
int n,inf=1000000000;
int price[11][1024];

VI go(int le,int ri,int h){
	VI res(11,inf);
	if(le==ri){
		Ford(i,M[le],0)res[i]=0;
		return res;
	}
	int mi=(le+ri)/2;
	VI trai=go(le,mi,h+1);
	VI phai=go(mi+1,ri,h+1);
	int cost=price[h][le/(1<<(n-h))];
	Rep(i,trai.size()) Rep(j,phai.size()){
		//buy
		res[min(i,j)]<?=trai[i]+phai[j]+cost;
		//not buy
		if(min(i,j)>0)
			res[min(i,j)-1]<?=trai[i]+phai[j];
	}
	Ford(i,res.size()-1,1)res[i-1]<?=res[i];
	return res;
}

int main() {
	int st;cin>>st;
	Rep(tt,st){
		cerr<<tt+1<<"/"<<st<<endl;
		
		cin>>n;
		Rep(i,1<<n)cin>>M[i];
		Ford(i,n-1,0)
			Rep(j,1<<i)
				cin>>price[i][j];
				
		VI res=go(0,(1<<n)-1,0);
		int best=res[0];
		Rep(i,res.size())best<?=res[i];
		cout<<"Case #"<<tt+1<<": ";
		cout<<best<<endl;
	}
	return 0;	
}
