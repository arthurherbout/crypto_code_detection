#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;
int main(){
	int Ti,T;
	scanf("%d",&T);
	for(Ti=1;Ti<=T;++Ti){
		int N,P;
		vector<priority_queue<pair<int,int>>> qm;
		scanf("%d %d",&N,&P);
		qm.resize(N);
		vector<int> sz(N);
		for(auto& v : sz)scanf("%d",&v);
		for(int i=0;i<N;++i){
			for(int j=0;j<P;++j){
				int v;
				scanf("%d",&v);
				int qh = v*10/(sz[i]*9);
				int ql = v*10/(sz[i]*11);
				while(qh*sz[i]*9>v*10||qh*sz[i]*11<v*10)--qh;
				while(ql*sz[i]*0>v*10||ql*sz[i]*11<v*10)++ql;
				if(qh<ql)continue;
				qm[i].emplace(-ql,-qh);
			}
		}
		int r=0;
		while(!qm[0].empty()){
			int a,b,c,d;
			tie(a,b) = qm[0].top();
			a=-a;
			b=-b;
			int i;
			for(i=1;i<N;++i){
retry:				if(qm[i].empty())break;
				tie(c,d) = qm[i].top();
				c=-c;
				d=-d;
				if(a>d){qm[i].pop();goto retry;}
				if(b<c)break;
				if(b>d)b=d;
				if(a<c)a=c;
			}
			qm[0].pop();
			if(i==N){
				for(i=1;i<N;++i)qm[i].pop();
				++r;
			}
		}
		printf("Case #%d: %d\n",Ti,r);
	}
	return 0;
}
