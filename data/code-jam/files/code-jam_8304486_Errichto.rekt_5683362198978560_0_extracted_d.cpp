#include <bits/stdc++.h>
using namespace std;
#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(x...) " [" #x ": " << (x) << "] "

#define int long long
typedef long long ll;
const ll mod = 1e9 + 7;

ll answer;

ll mul(ll a, ll b) {
	return a % mod * (b % mod) % mod;
}

void add(ll x) {
	answer = (answer + x) % mod;
}

ll f_sum(ll low, ll high) {
	ll a = low + high;
	ll b = high - low + 1;
	if(a % 2 == 0) a /= 2;
	else { assert(b % 2 == 0); b /= 2; }
	return mul(a, b);
}

ll f_cube(ll n) {
	ll a = n, b = n + 1, c = 2 * n + 1;
	if(a % 2 == 0) a /= 2;
	else if(b % 2 == 0) b /= 2;
	else c /= 2;
	if(a % 3 == 0) a /= 3;
	else if(b % 3 == 0) b /= 3;
	else c /= 3;
	return mul(mul(a, b), c);
}

ll f_cube(ll low, ll high) {
	ll one = f_cube(high);
	if(low > 0) {
		one = (one - f_cube(low - 1)) % mod + mod;
		one %= mod;
	}
	return one;
}

void test_case() {
	int h, w, n, cost;
	scanf("%lld%lld%lld%lld", &h, &w, &n, &cost);
	set<int> _rows{1, h+1}, _cols{1,w+1};
	vector<vector<int>> in(n);
	for(vector<int> & vec : in) {
		int row, col, b;
		scanf("%lld%lld%lld", &row, &col, &b);
		vec = {row, col, b};
		_rows.insert(row);
		_cols.insert(col);
	}
	for(auto a : in) for(auto b : in) {
		int dist = abs(a[0] - b[0]) + abs(a[1] - b[1]);
		if((long long) dist * cost < abs(a[2] - b[2])) {
			puts("IMPOSSIBLE");
			return;
		}
	}
	vector<int> rows, cols;
	for(int x : _rows) rows.push_back(x);
	for(int x : _cols) cols.push_back(x);
	
	auto ff = [&] (int row, int col) {
		long long big = -1;
		for(vector<int> & vec : in) {
			int dist = abs(vec[0] - row) + abs(vec[1] - col);
			ll maybe = (ll) dist * cost + vec[2];
			if(big == -1 || maybe < big) big = maybe;
		}
		return big;
	};
	
	answer = 0;
	
	for(int i0 = 0; i0 < (int) rows.size() - 1; ++i0)
		for(int j0 = 0; j0 < (int) cols.size() - 1; ++j0) {
			 int row1 = rows[i0], row2 = rows[i0+1] - 1;
			 int col1 = cols[j0], col2 = cols[j0+1] - 1;
			assert(row1 <= row2 && col1 <= col2);
			//~ debug() << imie(row1) imie(row2) imie(col1) imie(col2);
			
			 ll a = ff(row1, col1), b = ff(row1, col2);
			 ll c = ff(row2, col1), d = ff(row2, col2);
			
			//~ auto score = [&] (int row, int col) {
				//~ return min({
					//~ a + (ll) cost * (abs(row - row1) + abs(col - col1)),
					//~ b + (ll) cost * (abs(row - row1) + abs(col - col2)),
					//~ c + (ll) cost * (abs(row - row2) + abs(col - col1)),
					//~ d + (ll) cost * (abs(row - row2) + abs(col - col2))
				//~ });
				//~ 
			//~ };
			
			auto order = [&] (int x, int y, int z) { return x <= y && y <= z; };
			
			auto a_best = [&] (int row, int col) {
				if(!order(row1, row, row2) || !order(col1, col, col2)) return false;
				return a + (ll) cost * (abs(row - row1) + abs(col - col1))
				 <=
				 min({
					b + (ll) cost * (abs(row - row1) + abs(col - col2)),
					c + (ll) cost * (abs(row - row2) + abs(col - col1)),
					d + (ll) cost * (abs(row - row2) + abs(col - col2))
				});
			};
			
			//~ for(int row = row1; row <= row2; ++row)
				//~ for(int col = col1; col <= col2; ++col) {
					//~ debug() << imie(row) << imie(col) << imie(score(row, col));
					//~ answer += score(row, col);
				//~ }
			
			//~ row1 = 1, col1 = 1, row2 = 10, col2 = 30;
			//~ cost = 10;
			//~ a = -25000;
			//~ b = 0;
			//~ c = 0;
			//~ d = -250;
			
			for(int rep = 0; rep < 4; ++rep) {
				
				debug() << "--";
				debug() << imie(a) imie(b) imie(c) imie(d)   imie(row1) imie(row2) imie(col1) imie(col2);
				debug() << imie(cost);
				if(a_best(row1, col1)) {
				
					// increasing
					int low = 0, high = max(row2 - row1, col2 - col1);
					while(low < high) {
						int len = (low + high + 1) / 2;
						if(a_best(row1 + len, col1) && a_best(row1, col1 + len)) low = len;
						else high = len - 1;
					}
					debug() << "first" imie(low);
					
					
					add(mul(a, low + 1));
					add(mul(a + cost, f_sum(0, low)));
					add(mul(cost, f_cube(0, low)));
					//~ for(int i = 0; i <= low; ++i) {
						//~ answer += (ll) i * (a + cost);
						//~ answer += (ll) a;
						//~ answer += (ll) i * cost;
						//~ answer += (ll) i * i * cost;
					//~ }
						//~ answer += (ll) (i + 1) * (a + (ll) i * cost);
					
					int memo = low;
					
					low = 0, high = max(row2 - row1, col2 - col1);
					while(low < high) {
						int len = (low + high + 1) / 2;
						if(a_best(row1 + len, col1) || a_best(row1, col1 + len)) low = len;
						else high = len - 1;
					}
					debug() << "second" imie(low);
					assert(memo <= low);
					add(  mul(  mul(low - (memo + 1) + 1,  memo + 1), a));
					add( mul( mul(memo + 1, cost),  f_sum(memo + 1, low)));
					//~ for(int i = memo + 1; i <= low; ++i)
						//~ answer += (ll) (memo + 1) * ((ll) i * cost);
					
					int memo2 = low;
					
					low = memo2, high = row2 - row1 + col2 - col1;
					while(low < high) {
						int len = (low + (ll) high + 1) / 2;
						if(a_best(row1 + memo2, col1 + (len - memo2))
							|| a_best(row1 + (len - memo2), col1 + memo2)) low = len;
						else high = len - 1;
					}
					debug() << "third" imie(low);
					assert(memo2 <= low);
					add(  mul( low - (memo2 + 1) + 1,  mul(a, memo + 1 + memo2)));
					add( mul(cost, mul(memo + 1 + memo2, f_sum(memo2 + 1, low))));
					add( mul(-a, f_sum(memo2 + 1, low)));
					add(mul(-cost, f_cube(memo2 + 1, low)));
					//~ for(int i = memo2 + 1; i <= low; ++i)
						//~ answer += (ll) (-i) * ((ll) i * cost);
					
					//~ auto a_memo = a;
					//~ a = b;
					//~ b = d;
					//~ d = c;
					//~ d = a_memo;
				}
				
				a--;
				
				if(rep % 2 == 0) {
					swap(a, b);
					swap(c, d);
					//~ swap(col1, col2);
				}
				if(rep == 1) {
					swap(a, c);
					swap(b, d);
					//~ swap(row1, row2);
				}
			}
			
			//~ exit(0);
			
			//~ if(a + d > b + c) {
				//~ swap(a, c);
				//~ swap(b, d);
			//~ }
			//~ 
			//~ int low, high;
			//~ 
			//~ low = col1, high = col2;
			//~ while(low < high) {
				//~ int col = (low + high + 1) / 2;
				//~ if(a_best(row1, col)) low = col;
				//~ else high = col - 1;
			//~ }
			//~ 
			//~ int last_col = low;
			//~ 
			//~ low = row1, high = row2;
			//~ while(low < high) {
				//~ int row = (low + high + 1) / 2;
				//~ if(a_best(row, last_col)) low = row;
				//~ else high = row - 1;
			//~ }
			//~ 
			//~ int mid_row = low;
			//~ 
			//~ low = 0, high = min(last_col - col1, row2 - mid_row);
			//~ while(low < high) {
				//~ int dist = (low + high + 1) / 2;
				//~ if(a_best(mid_row + dist, last_col - dist)) low = dist;
				//~ else high = dist-1;
			//~ }
			//~ int last_row = mid_row + low;
			//~ int mid_col = last_col - low;
			
			
			
			//~ for(int repeat = 0; repeat < 4; ++repeat) {
				//~ 
			//~ }
		}
	
	answer = answer % mod + mod;
	printf("%lld\n", answer % mod);
}

#undef int
int main() {
	int T;
	scanf("%d", &T);
	for(int nr = 1; nr <= T; ++nr) {
		printf("Case #%d: ", nr);
		test_case();
	}
}
