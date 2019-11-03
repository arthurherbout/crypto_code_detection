#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cassert>
#include<algorithm>
#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<set>
#include<map>
#include<queue>

using namespace std;

#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,(n)-1)

#define pb push_back
#define mp make_pair

typedef long long ll;
typedef pair<int,int> pint;

int in(){int a; scanf("%d",&a); return a;}
int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};

typedef pair<pint,int> tint;
#define one first.first
#define two first.second
#define thr second


int main(){
	int ttt=in();
	rep(cas,ttt){
		tint c[100];
		int n=in();
		priority_queue<tint> pq;
		rep(i,n){ c[i].one=in(); c[i].thr=in(); c[i].two=in(); pq.push(c[i]); }
		int m=in();
		rep(i,m){ c[n+i].one=in(); c[n+i].thr=in(); c[n+i].two=in(); }
//		sort(c,c+m+n);
		int sc=0; int pp=n; int tr=1;
//		cout << "hoge" << endl;
		while(!pq.empty()){
			tint cc=pq.top();
			if(cc.one && cc.two){
				sc += cc.thr;
				tr += cc.two-1;
				pq.pop();
				if(cc.one && pp<n+m) pq.push(c[pp++]);
			}else break;
		}
		priority_queue<tint> pq2;
		while(!pq.empty()){
			tint cc=pq.top();
			if(cc.one){
				pq2.push(cc);
				pq.pop();
			}else break;
		}
		while(!pq.empty()){
			tint cc=pq.top();
			if(cc.two){
				sc += cc.thr;
				tr += cc.two-1;
				pq.pop();
			}else break;
		}
//		cout << "tr:" << tr << endl;
		int res=0;
		repp(i,0,tr){
			priority_queue<tint> q(pq),q2(pq2);
			int ppp=pp;
			int scc=sc;
			rep(j,i){
				if(!q2.empty()){
					if(ppp<n+m) q.push(c[ppp++]);
					scc += q2.top().thr;
					q2.pop();
				}
			}
//			while(!q.empty() && q.top().two) q.pop();
			rep(j,tr-i){
				if(!q.empty()){
					scc += q.top().thr;
					q.pop();
				}
			}
			res = max(res, scc);
		}
		cout << "Case #" << cas+1 << ": " << res << endl;
	}
    return 0;
}

