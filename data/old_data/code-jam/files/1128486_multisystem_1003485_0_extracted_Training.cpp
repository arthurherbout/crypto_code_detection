#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <list>
#include <climits>
#include <assert.h>
#define rep(i,n) for(int i=0; i<n; i++)
#define reps(i,m,n) for(int i=m; i<n; i++)
#define repe(i,m,n) for(int i=m; i<=n; i++)
#define repi(it,stl) for(typeof((stl).begin()) it = (stl).begin(); (it)!=stl.end(); ++(it))
#define sz(a) ((int)(a).size())
#define mem(a,n) memset((a), (n), sizeof(a))
#define all(n) (n).begin(),(n).end()
#define rall(n) (n).rbegin(),(n).rend()
#define allarr(n) (n), (n)+( (sizeof (n)) / (sizeof (*n)) )
#define mp(a,b) make_pair((a),(b))
#define pnt(a,b) mp((a),(b))
#define pii pair<int,int>
#define pib pair<int,bool>,&n
#define vi vector<int>
#define vc vector<char>
#define vs vector<string>
#define sstr stringstream
typedef long long ll;
using namespace std;

int l, t, n, c;

int inp[1000005];


void buildMap(vi& arr, map<int, int>& m, int idx) {
	reps(i,idx+1,sz(arr)) {
		++m[arr[i]];
	}
}
int main() {

	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);

	int tst;
	scanf("%d", &tst);
	repe(tt,1,tst) {
		scanf("%d%d%d%d", &l, &t, &n, &c);
		printf("Case #%d: ", tt);

		int res = 0;
		vi arr;
		rep(i,c) {
			scanf("%d", &inp[i]);
			arr.push_back(inp[i]*2);
		}
		int idx = 0;
		while (sz(arr) < n) {
			arr.push_back(inp[idx++] * 2);
			if (idx == c)
				idx = 0;
		}

		bool frst = 1;
		map<int, int> freq, newfreq;
		rep(i,sz(arr)) {
			if (l && (res + arr[i] > t)) {
				if (frst) {
					frst = false;
					int dst = (t - res) + ((arr[i] / 2) - ((t - res) / 2));
					buildMap(arr, freq, idx);
					freq[dst]++;
					map<int, int>::iterator it = freq.end();
					--it;
					int siz = 0;
					while (true) {
						siz += it->second;
						newfreq[it->first] = it->second;
						if (siz >= l){
							newfreq[it->first] -= siz - l;
							siz = l;
							break;
						}

						if (it == freq.begin())
							break;
						--it;
					}

					if (siz < l)
						res += dst, newfreq[dst]--;
					else if(dst > freq.begin()->first)
						res += dst, newfreq[dst]--;
					else
						res += arr[i];


				} else if (newfreq[arr[i]] > 0) {
					res += arr[i]/2;
					newfreq[arr[i]]--;
				}
				else
					res += arr[i];

			} else
				res += arr[i];
		}

		printf("%d\n",res);

	}

	return 0;
}

