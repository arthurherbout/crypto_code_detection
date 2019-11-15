#include <iostream>
#include <queue>
#include <map>
using namespace std;
typedef unsigned long long u64;
int main(){
	int Ti,T;
	cin>>T;
	for(Ti=1;Ti<=T;++Ti){
		map<u64,u64> q;
		u64 n,k;
		cin>>n>>k;
		q[n]=1;
		u64 v,c;
		while(!q.empty()){
			auto end = q.end();
			tie(v,c) = *--end;
			q.erase(v);
			if(c >= k) break;
			k-=c;
			--v;
			if(~v&1)
				q[v/2]+=c<<1;
			else
				q[v/2]+=c, q[v-v/2]+=c;
		}
		--v;
		cout<<"Case #"<<Ti<<": "<<v-v/2<<' '<<v/2<<'\n';
	}
}
