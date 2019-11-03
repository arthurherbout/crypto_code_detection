// _temp.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

#define MAXN 10000

struct point {
	int x, y;
	int id;
} p[MAXN], p2[MAXN], pi[MAXN];

bool pless(const point &a, const point &b) {
	return a.y < b.y || (a.y==b.y && a.x > b.x);
}

bool pinit(const point &a, const point &b) {
	return a.y > b.y || (a.y==b.y && a.x < b.x);
}


ll SQR(ll x) {
	return x*x;
}

int n, k;
bool nn[MAXN];
vector<int> ans;
vector<int> l;
vector<int> r;
vector<int> up;
vector<int> down;

ll px, py;

int low_y, best_i;

ll calc_sq(vector<int> a) {	
	int a0 = a[0];
	ll sum = 0;
	for(int i=1; i<a.size() - 1; ++i) {
        int a1 = a[i];
		int a2 = a[i + 1];
		ll tmp = (pi[a2].x - pi[a0].x)*(pi[a1].y - pi[a0].y) - 
			     (pi[a2].y - pi[a0].y)*(pi[a1].x - pi[a0].x);
		sum += tmp;
	}
	if (sum < 0) sum = -sum;
	return sum;
}

ll tmp_sq;
ll max_sq;

int main()
{
	ll INF = 1;
	for(int i=0; i<15; ++i) INF = INF*10;

    int tc;
    //freopen("a.in", "r", stdin);
	freopen("B-small-attempt2.in", "r", stdin);
    freopen("B-small-attempt2.out", "w", stdout);
    scanf("%i", &tc);
    for(int tt=1; tt<=tc; ++tt) {
		fprintf(stderr, "tc = %i\n", tt);
		scanf("%i", &n);
		for(int i=0; i<n; ++i) {
			int x, y;
			scanf("%i %i", &x, &y);
			p[i].x = x;
			p[i].y = y;
			p[i].id = i;
			pi[i] = p[i];
		}

		low_y = -INF;
		for(int i=0; i<n; ++i) {
			//if (hi_y < p[i].y) hi_y = p[i].y;
			if (low_y > p[i].y) low_y = p[i].y;
		}

		sort(p, p+n, pinit);
		ans.clear();
		up.clear();
		down.clear();

		for(int i=0; i<n; ++i) nn[i] = false;

		k = 0;
		while (k < n && p[k].y == p[0].y) {
			up.push_back( p[k].id );		    
			k++;
		}		

		k = n - 1;
		while (k >= 0 && p[k].y == p[n - 1].y) {
			down.push_back( p[k].id );		    
			k--;
		}		

		// right side
		r.clear();
		for(int i=0; i<n; ++i) nn[i] = false;
		for(int i=0; i<up.size(); ++i) nn[ up[i] ] = true;
		px = pi[ up[ up.size() - 1] ].x;
		py = pi[ up[ up.size() - 1] ].y;
		while (true) {
			if (py == low_y) break;

			ll best_sc = -INF;
			ll best_len = INF;
			int q;

			for(int i=0; i<n; ++i) if (!nn[p[i].id]) { 
				best_len = SQR(px - p[i].x) + SQR(py - p[i].y);
				q = i; 
			    break; 
			}

			for(int i=0; i<n; ++i) if (!nn[p[i].id]) {
				ll len = SQR(px - p[i].x) + SQR(py - p[i].y);
				ll sc  = (p[i].x - px)*(p[q].y - py) - (p[i].y - py)*(p[q].x - px);

				if (sc < 0 || (sc == 0 && len < best_len)) {					
					best_len = len;
					q = i;
				}
			}

			nn[p[q].id] = true;
			px = p[q].x;
			py = p[q].y;
			if (py == low_y) break;
			r.push_back( p[q].id );
		}


		// left side
		l.clear();
		for(int i=0; i<n; ++i) nn[i] = false;
		for(int i=0; i<up.size(); ++i) nn[ up[i] ] = true;
		px = pi[ up[0] ].x;
		py = pi[ up[0] ].y;
		while (true) {
			if (py == low_y) break;

			ll best_sc = -INF;
			ll best_len = INF;
			int q;

			for(int i=0; i<n; ++i) if (!nn[p[i].id]) { 
				best_len = SQR(px - p[i].x) + SQR(py - p[i].y);
				q = i; 
			    break; 
			}

			for(int i=0; i<n; ++i) if (!nn[p[i].id]) {
				ll len = SQR(px - p[i].x) + SQR(py - p[i].y);
				ll sc  = (p[i].x - px)*(p[q].y - py) - (p[i].y - py)*(p[q].x - px);

				if (sc > 0 || (sc == 0 && len < best_len)) {					
					best_len = len;
					q = i;
				}
			}

			nn[p[q].id] = true;
			px = p[q].x;
			py = p[q].y;
			if (py == low_y) break;
			l.push_back( p[q].id );
		}



		// make cool
		ans.clear();
		for(int i=0; i<up.size(); ++i) ans.push_back( up[i] );
	    for(int i=0; i<r.size(); ++i) ans.push_back( r[i] );
		for(int i=0; i<down.size(); ++i) ans.push_back( down[i] );
		for(int i=l.size() - 1; i>=0; --i) ans.push_back( l[i] );

		max_sq = calc_sq(ans);
		assert(max_sq != 0);

		// make right
		ans.clear();
		for(int i=0; i<n; ++i) nn[i] = false;
		for(int i=0; i<up.size(); ++i) ans.push_back( up[i] ), nn[ up[i] ] = true;
	    for(int i=0; i<r.size(); ++i) ans.push_back( r[i] ), nn[ r[i] ] = true;
		for(int i=0; i < down.size(); ++i) ans.push_back( down[i] ), nn[ down[i] ] = true;
		k = 0;
		for(int i=0; i<n; ++i) if (!nn[p[i].id]) p2[k++] = p[i];
		sort(p2, p2 + k, pless);
		for(int i=0; i<k; ++i) ans.push_back( p2[i].id );

		tmp_sq = calc_sq(ans);
		if (tmp_sq*2 <= max_sq) {
			// make left
			ans.clear();
			for(int i=0; i<n; ++i) nn[i] = false;
			for(int i=up.size() - 1; i >= 0; --i) ans.push_back( up[i] ), nn[ up[i] ] = true;
			for(int i=0; i<l.size(); ++i) ans.push_back( l[i] ), nn[ l[i] ] = true;
			for(int i=down.size() - 1; i >= 0; --i) ans.push_back( down[i] ), nn[ down[i] ] = true;
			k = 0;
			for(int i=0; i<n; ++i) if (!nn[p[i].id]) p2[k++] = p[i];
			sort(p2, p2 + k, pless);
			for(int i=0; i<k; ++i) ans.push_back( p2[i].id );

			tmp_sq = calc_sq(ans);

			if (tmp_sq*2 <= max_sq) {
				assert(false);
			}

		}


        printf("Case #%i:", tt);
		if (ans.size() != n) printf("FUCK!!!!!!!!!!!!!!!\n");
		for(int i=0; i<ans.size(); ++i) {
			printf(" %i", ans[i]);
		}
		printf("\n");
    }
    return 0;
}

