#include<bits/stdc++.h>
#include<string>
using namespace std;
template< class T > T gcd(T a, T b) { return (b != 0 ? gcd<T>(b, a%b) : a); }
template< class T > T lcm(T a, T b) { return (a / gcd<T>(a, b) * b); }
#define traverse(container, it) \
  for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)
#define         mp(x, y) make_pair(x, y)
#define         REV(s, e) reverse(s, e)
#define         CPY(d, s) memcpy(d, s, sizeof(s))
#define         READ(f) freopen(f, "r", stdin)
#define         WRITE(f) freopen(f, "w", stdout)
#define         ALL(c) c.begin(), c.end()
#define         SIZE(c) (int)c.size()
#define         pb(x) push_back(x)
//#define       map<char,int>::iterator it;
#define         ff first
#define         ss second
#define         ll long long
#define         ld long double
#define         pii pair< int, int >
#define         psi pair< string, int >
#define         p(n) printf("%d\n",n)
#define         p64(n) printf("%lld\n",n)
#define         s(n) scanf("%d",&n)
#define         s64(n) scanf("%I64d",&n)
#define         rep(i,a,b) for(i=a;i<b;i++)
#define         MOD (1000000007LL)

/////////////////////////////////////////////////



ll n,m,p;

ll solve(){
	ll ans  = 10000;
	if(p==0) return 0;
	ll mask  = 0;
	rep(mask ,0,(1<<n*m)){


		if(__builtin_popcount(mask) != p)	continue;
		ll lans = 0;
		ll j = 0;
		rep(j,0,n*m){

			if((mask&(1<<j))==0) continue;

				int r = j/m;
				int c = j%m;

				int ind = (r+1)*m + c;

				if(r<n-1&&((mask>>ind)&1))	lans++;

				ind = r*m + c+1;

				if(c<m-1&&((mask>>ind)&1))	lans++;


			

			
		}
	ans = min(ans,lans);

	}
return ans;


}

int main(){
	std::ios::sync_with_stdio(false);
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	
	
	int t;
	cin>>t;
	int k = 1;
	//recur(1);
	solve();
	
	while(t--){
		cin>>n>>m>>p;

		cout<<"Case #"<<k++<<":"<<" "<<solve()<<endl;;




	}

return 0;
}



