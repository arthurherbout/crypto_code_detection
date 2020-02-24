#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cassert>
using namespace std;
typedef long long ll;
typedef double R;
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define FOR(i, s, t) for(i = (s); i < (t); i++)
#define RFOR(i, s, t) for(i = (s)-1; i >= (t); i--)

void debug1(){}

template<class T, class ... Tails>
void debug1(const T &v, const Tails& ... tails){
	cout<<" "<<v;
	debug1(tails...);
}

template<class T, class ... Tails>
void debug(const T &v, const Tails& ... tails){
#ifdef LOCAL
	cout<<"Debug: "<<v;
	debug1(tails...);
	cout<<endl;
#endif
}

const R PI = acos(-1);
const int MAXN = 5123;
const int P = 1e9+7;
const R EPS = 1e-9;
const string no = "NO";
const string yes = "YES";

struct Z3{
	ll x, y, z;

	Z3(){}
	Z3(ll x, ll y, ll z):x(x), y(y), z(z){}

	friend istream& operator>> (istream& in, Z3 &a) {
		return in>>a.x>>a.y>>a.z;
	}

	Z3 operator- (const Z3 &a) const{
		return Z3(x-a.x, y-a.y, z-a.z);
	}

	Z3 cross(const Z3 &a) const{
		return Z3(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
	}

	ll dot(const Z3 &a) const{
		return (ll)x*a.x + (ll)y*a.y + (ll)z*a.z;
	}

	bool operator== (const Z3 &a) const{
		return x==a.x && y==a.y && z==a.z;
	}

};

Z3 a[MAXN];


int n;

int main(){
#ifdef LOCAL
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
#endif
	int T, i0;
	scanf("%d", &T);
	for(i0 = 1; i0 <= T; i0++){
		int i, j, k;
		string ans = yes;
		bool ok = false;

		cin>>n;
		for(i = 0; i < n; i++)
			cin>>a[i];

		for(i = 0; i < n; i++){
			bool dup = false;
			for(j = 0; j < i; j++){
				Z3 d = a[i].cross(a[j]);
				if(d == Z3(0, 0, 0)){
					if(a[i].dot(a[j]) > 0)
						dup = true;
					else
						ok = true;
				}
			}
			if(dup){
				a[i] = a[n-1];
				n--;
				i--;
			}
		}
		if(!ok){
			for(i = 0; i < n; i++)
				for(j = 0; j < n; j++)
					if(i != j){
						Z3 d = a[i].cross(a[j]);
						bool f = true;
						for(int x = 0; x < n; x++)
							if(x != i && x != j)
								f &= a[x].dot(d) > 0;
						if(f)
							ans = no;
					}
		}

		printf("Case #%d: %s\n", i0, ans.c_str());
	}
	return 0;
}
