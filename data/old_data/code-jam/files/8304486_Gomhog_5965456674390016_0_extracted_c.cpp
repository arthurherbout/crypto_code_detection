#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
#define F first
#define S second

int e[33],l[33],d[33];
int vis[17];
const ll INF=1e16;


int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	for (int tc=1;tc<=tcs;tc++) {
		int c;
		cin>>c;
		for (int i=0;i<2*c;i++) {
			cin>>e[i]>>l[i]>>d[i];
			e[i]--;
		}
		ll parsa=INF;
		for (int mask=0;mask<(1<<c);mask++) {
			for (int i=0;i<c;i++) vis[i]=0;
			ll len=0;
			int cur=0;
			while (vis[cur]<2) {
				int hlp=vis[cur];
				if ((mask&(1<<cur))) hlp=1-hlp;
				vis[cur]++;
				int j=2*cur+hlp;
				len+=(l[j]-(len%24)+24)%24+d[j];
				cur=e[j];
			}
			bool ok=true;
			for (int i=0;i<c;i++) {
				if (vis[i]<2) ok=false;
			}
			if (ok) parsa=min(parsa,len);
		}
		cerr<<tc<<endl;
		cout<<"Case #"<<tc<<": "<<parsa<<"\n";
	}
}
