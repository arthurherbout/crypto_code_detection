#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <cstdlib>
#include <memory>
#include <queue>
#include <cassert>
#include <cmath>
#include <ctime>

using namespace std;

#define pb push_back
#define f first
#define s second
#define mp make_pair 
#define sz(C) ((int) (C).size())
#define forn(i, n) for (int i = 0; i < (int) n; ++i)
#define ford(i, n) for (int i = ((int) n) - 1; i >= 0; --i)
#define foran(i, a, n) for (int i = (int) a; i < (int) n; ++i)
#define y1 gftxdtrtfhyjfctrxujkvbhyjice
#define y0 ehfoiuvhefroerferjhfjkehfjke
#define left sdhfsjkshdjkfsdfgkqqweqweh
#define right yytrwtretywretwreytwreytwr
#define all(C) (C).begin(), (C).end()

template <typename T>
inline T sqr( T x ) {
	return x * x;
}

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef pair <int,int> pii;
typedef pair <ll, ll> pll;
typedef vector <ll> vll;
typedef vector <int> vi;
typedef vector <vector <int> > vvi;
typedef vector <pii> vii;

const double EPS = 1e-9;
const int MOD = int(1e9) + 7;

inline int mul( int a, int b ) {
	return a * 1ll * b % MOD;
}

inline int code( char c ) {
	return int(c - 'a');
}

string s[110];
int occur[30];
bool was[30];
int cnt[30][30];
int fact[200];
int full[30];
int ncnt[30][30];
bool WAS[30];

void precalc() {
	fact[0] = 1;
	for (int i = 1; i < 200; ++i) {
		fact[i] = mul(fact[i - 1], i);
	}	
}

int main() {
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	precalc();

	int t;
	scanf("%d", &t);
	forn(it, t) {
		int n;
		scanf("%d\n", &n);

		memset (occur, 0, sizeof occur);
		forn(i, n) {
			cin >> s[i];

			memset (was, false, sizeof was);

			forn(j, sz(s[i])) {
				was[code(s[i][j])] = true;	
			}

			forn(c, 30) {
				occur[c] += int(was[c]);
			}
		}

//		if  (it != 9) {
//			continue;
//		}

//		forn(c, 30)
//			if  (occur[c]) {
//				printf("occur[%d] = %d\n", c, occur[c]);
//			}

		memset (WAS, false, sizeof WAS);
		memset (cnt, 0, sizeof cnt);
		memset (full, 0, sizeof full);
		bool ok = true;
		forn(i, n) {
			string cur = s[i];
			char st = cur[0];
			int left = 0;

			while (left < sz(cur) && cur[left] == st) ++left;

		   	if  (left == sz(cur)) {
		   		++full[code(st)];
		   		continue;
		   	}

		   	int right = sz(cur) - 1;
		   	char end = cur[sz(cur) - 1];
		   	while (right >= 0 && cur[right] == end) --right;

//		   	printf("[%d, %d]\n", left, right);
		   	for (int j = left; j <= right; ++j)
		   		if  (occur[code(cur[j])] > 1) {
		   			ok = false;
		   		}

			memset (was, false, sizeof was);
			forn(j, sz(cur)) {
				int ch = code(cur[j]);

				if  (was[ch] && cur[j - 1] != cur[j]) {
					ok = false;
				}

				was[ch] = true;
			}

			if  (st == end) {
				assert(!ok);
			} else {
				++cnt[code(st)][code(end)];
				WAS[code(st)] = true;
				WAS[code(end)] = true;
			}	
		}	

		if  (!ok) {
			printf("Case #%d: 0\n", it+1);
			continue;
		}
	
//	    forn(c, 30)
//	    	if  (full[c]) {
//	    		printf("%d -> %d\n", c, full[c]);
//	    	}

		while (1) {
//			puts("GO");
			forn(left, 30) {
				bool have = false;
				forn(right, 30)
					if  (cnt[left][right]) {
//						printf("cnt[%d][%d] = %d\n", left, right, cnt[left][right]);
						if  (have) {
							ok = false;
						}
						have = true;
					}
			}

			if  (!ok) {
//				puts("gavno");
				break;
			}

			forn(right, 30) {
				bool have = false;
				forn(left, 30)
					if  (cnt[left][right]) {
						if  (have) {
							ok = false;
						}
						have = true;
					}
			}

			if  (!ok) {
				break;
			}

			forn(l, 30)
				forn(r, 30) {
					if  (cnt[l][r] > 1) {
						ok = false;
					}             
				}

			if  (!ok) {
				break;
			}
/*
			forn(l, 30)
				forn(r, 30) {
					if  (cnt[l][r]) {
						printf("cnt[%d][%d] = %d\n", l, r, cnt[l][r]);
					}
				}

*/
			bool cont = false;
			for (int center = 0; center < 30 && !cont; ++center) {
				for (int left = 0; left < 30 && !cont; ++left)
					for (int right = 0; right < 30 && !cont; ++right)
						if  (cnt[left][center] && cnt[center][right]) {
//							printf("%d %d\n", ncnt[left][center], ncnt[center][right]);
							--cnt[left][center];
							--cnt[center][right];
							++cnt[left][right];
							cont = true;
							break;
						}
			}

			forn(c, 30) {
				if  (cnt[c][c]) {
					ok = false;
				}
			}

			if  (!cont) {
				break;
			}
		}

//		puts("after:");
//			forn(l, 30)
//				forn(r, 30) {
//					if  (cnt[l][r]) {
//						printf("cnt[%d][%d] = %d\n", l, r, cnt[l][r]);
//					}
//				}
	
		forn(left, 30) {
			bool have = false;
			forn(right, 30)
				if  (cnt[left][right]) {
					if  (have) {
						ok = false;
					}
					have = true;
				}
		}

		forn(right, 30) {
			bool have = false;
			forn(left, 30)
				if  (cnt[left][right]) {
					if  (have) {
						ok = false;
					}
					have = true;
				}
		}

		forn(l, 30)
			forn(r, 30)
				if  (cnt[l][r] > 1) {
					ok = false;
					break;
				}

		if  (!ok) {
			printf("Case #%d: 0\n", it+1);
			continue;
		}
		
		int all = 0;
		forn(l, 30)
			forn(r, 30)
				all += cnt[l][r];
/*
		forn(c, 30)
			if  (full[c]) {
				printf("full[%d] = %d\n", c, full[c]);
			}
*/
		int res = fact[all];
//		printf("res = %d\n", res);
		forn(c, 30) {
			if  (WAS[c]) {
			    res = mul(res, fact[full[c]]);
			} else {
				res = mul(res, fact[full[c]]);
				if  (full[c]) {
					res = mul(res, all + 1);
					++all;
				}
			}	

//			all += full[c];
		}

//		printf("res = %d\n", fact[3]);
		printf("Case #%d: %d\n", it+1, res);
	}

	return 0;
}
