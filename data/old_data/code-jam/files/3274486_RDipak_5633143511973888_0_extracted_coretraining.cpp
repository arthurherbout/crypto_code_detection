#include<bits/stdc++.h>
using namespace std;
#define err(x) cout<<#x<<"= "<<x<<endl;
#define FOR(i,a,b) for(int i =a; i< b; ++i)
#define rep(i,n) FOR(i,0,n)
#define pb push_back
#define INF 1000000000
#define TRVI(it,it1,it2) for(vi::iterator it = it1; it!= it2; it++)
#define ff first
#define ss second
#define mp make_pair
#define pq priority_queue<int, vector<int>, greater<int> >
#define ll long long
const ll PR = 1000000009;
#define SIZE 59
#define vi vector<int>
#define pii pair<int,int>
#define endl '\n'
#define db long double

ll pr[SIZE],U,pr1[SIZE];
int n,k;

int check(int now){
	ll avail = U;
	int cnt =0;
	rep(i,n){
		if(pr[i]<now){
			pr1[i] = now;
			avail-= now-pr[i];
			cnt++;
		}
		else pr1[i] = pr[i];
	}
	if(avail>=cnt)return 2;
	if(avail<0)return 0;
	if(avail>0){
		rep(i,n){
			pr1[i]++;
			avail--;
			if(avail<=0)break;
		}
	}
	return 1;
}

int main(){
//	#ifdef ONLINE_JUDGE
//	freopen("a.in", "r" , stdin);
//	freopen("a.out", "w", stdout);
//      cin.tie(false); cout.tie(false);	
//	#endif
	ios::sync_with_stdio(false);
	db u;
	int t;
	cin>>t;
	rep(t1,t){
		cin>>n>>k;
		cin>>u;
		U = u*1000000;
		rep(i,n){
			cin>>u;
			pr[i] = u*1000000;
		}
		sort(pr,pr+n);
		for(int i =pr[0]; i<=1000000; i++){
			if(check(i)== 1|| i==1000000)break;
		}
		db ans=1;
		rep(i, n){
			ans= ans*(min(pr1[i]/1000000.0,1.0));
		}
		cout<<"Case #"<<t1+1<<": "<<fixed<<setprecision(8)<<ans<<endl;
	}

	return 0;
};
