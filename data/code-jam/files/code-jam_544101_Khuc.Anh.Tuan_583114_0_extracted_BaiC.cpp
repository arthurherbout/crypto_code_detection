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

bool iswin(int a,int b){
	if(a==b)return false;
	if(a<b)swap(a,b);	
	if(a>=2*b)return true;
	return !iswin(b,a-b);
}

int a1,a2,b1,b2;

long long calc(int a1,int a2,int b1,int b2){
	long long res=0;
	For(i,a1,a2){
		int L=1,R=i;
		while(L<R){
			int M=(L+R)/2;
			if(!iswin(i,M))R=M;
			else L=M+1;	
		}	
		// L -> i-1
		int s=max(b1,L);
		int e=min(b2,i-1);
		if(e>=s)res+=e-s+1;
	}
	return res;
}

int main() {
	int st;
	scanf("%d",&st);
	Rep(t,st){
		scanf("%d%d%d%d",&a1,&a2,&b1,&b2);
		int bang=0;
		For(i,a1,a2)if(b1<=i&&i<=b2)++bang;
		long long res=bang;
		res+=calc(a1,a2,b1,b2);
		res+=calc(b1,b2,a1,a2);
		long long total=(a2-a1+1)*1LL*(b2-b1+1);
		cout << "Case #" << t+1 << ": ";
		cout<<total-res;
		cout<<endl;
	}
	return 0;	
}
