#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
#define F first
#define S second

const ll mod=1e9+7;
ll grid[201][201];
bool handled[201][201];

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	for (int tc=1;tc<=tcs;tc++) {
		int r,c,n;
		ll d;
		cin>>r>>c>>n>>d;
		for (int i=0;i<r;i++) {
			for (int j=0;j<c;j++) {
				grid[i][j]=-1;
				handled[i][j]=false;
			}
		}
		priority_queue<pair<ll,pair<int,int>>> pq;
		for (int i=0;i<n;i++) {
			int ri,ci;
			ll bi;
			cin>>ri>>ci>>bi;
			pq.push({-bi,{ri-1,ci-1}});
			grid[ri-1][ci-1]=bi;
		}
		bool ok=true;
		ll sum=0;
		while (!pq.empty()) {
			ll bi=-pq.top().F;
			int i=pq.top().S.F;
			int j=pq.top().S.S;
			pq.pop();
			if (handled[i][j]) continue;
			if (bi<grid[i][j]) {
				ok=false;
				break;
			}
//			cerr<<i<<" "<<j<<endl;
			grid[i][j]=bi;
			handled[i][j]=true;
			sum+=bi;
			sum%=mod;
			if (i>0) pq.push({-bi-d,{i-1,j}});
			if (j>0) pq.push({-bi-d,{i,j-1}});
			if (i<r-1) pq.push({-bi-d,{i+1,j}});
			if (j<c-1) pq.push({-bi-d,{i,j+1}});
		}
		cerr<<tc<<endl;
		cout<<"Case #"<<tc<<": ";
		if (ok) cout<<sum<<"\n";
		else cout<<"IMPOSSIBLE\n";
	}
}
