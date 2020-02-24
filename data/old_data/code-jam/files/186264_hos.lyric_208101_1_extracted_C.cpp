//	GCJ 2009 Round 1B (C)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <bitset>
#include <complex>

using namespace std;

typedef unsigned uint;
typedef long long Int;
typedef vector<int> vint;
typedef pair<int,int> pint;
#define mp make_pair

template<class T> void pv(T a, T b) { for (T i = a; i != b; ++i) cout << *i << " "; cout << endl; }
int in_c() { int c; while ((c = getchar()) <= ' ') { if (!~c) throw ~0; } return c; }
int in() {
	int x = 0, c;
	while ((uint)((c = getchar()) - '0') >= 10) { if (c == '-') return -in(); if (!~c) throw ~0; }
	do { x = (x << 3) + (x << 1) + (c - '0'); } while ((uint)((c = getchar()) - '0') < 10);
	return x;
}

const int LIM = 505;
const int HOS = 100;

int dx[] = {1, 0, -1, 0, };
int dy[] = {0, 1, 0, -1, };

int N, Q;
char S[22][22];

int qrys[10010];
int isqry[10010], lefts;
pint ans[10010];

int E;
pair< int,pair<pint,pint> > p[10010];
int val[10010];

int inilen;
pair<char,pint> ini[10010];

int prev[22][22][10010];
int lens[2];
pair<pint,int> que[2][4000010];
int same[2][4000010];

int L;
char buf[100010];

inline bool inside(int x, int y) {
	return (0 <= x && x < N && 0 <= y && y < N);
}

int main() {
	int x, y, g, h;
	int xo, yo, xx, yy;
	int a, b;
	int i, I, J, e;
	int befi, befe;
	int iq, ir;
	
	for (int TC = in(), tc = 0; TC--; ) {
		printf("Case #%d:\n", ++tc);
		
		N = in();
		Q = in();
		for (x = 0; x < N; ++x) {
			scanf("%s", S[x]);
		}
		memset(isqry, 0, sizeof(isqry));
		lefts = 0;
		for (i = 0; i < Q; ++i) {
			qrys[i] = HOS + in();
			if (!isqry[qrys[i]]) {
				isqry[qrys[i]] = 1;
				++lefts;
			}
		}
		
		
		E = 0;
		for (x = 0; x < N; ++x) for (y = 0; y < N; ++y) if (isdigit(S[x][y])) {
			for (g = 0; g < 4; ++g) if (inside(xo = x + dx[g], yo = y + dy[g])) {
				for (h = 0; h < 4; ++h) if (inside(xx = xo + dx[h], yy = yo + dy[h])) {
					p[E++] = mp(S[xo][yo] << 8 | S[xx][yy], mp(mp(x, y), mp(xx, yy)));
				}
			}
		}
		sort(p, p + E);
		for (e = 0; e < E; ++e) {
			val[e] = (p[e].first & 0xff) - '0';
			if ((p[e].first >> 8) == '-') val[e] = -val[e];
		}
		
		memset(prev, ~0, sizeof(prev));
		
		inilen = 0;
		for (x = 0; x < N; ++x) for (y = 0; y < N; ++y) if (isdigit(S[x][y])) {
			ini[inilen++] = mp(S[x][y], mp(x, y));
		}
		sort(ini, ini + inilen);
		
		iq = 0;
		ir = 1;
		lens[iq] = lens[ir] = 0;
		for (i = 0; i < inilen; ++i) {
			prev[ini[i].second.first][ini[i].second.second][a = HOS + (ini[i].first - '0')] = -2;
			same[iq][lens[iq]] = (i && ini[i - 1].first == ini[i].first) ? same[iq][lens[iq] - 1] : lens[iq];
			que[iq][lens[iq]++] = mp(ini[i].second, a);
			if (isqry[a]) {
				--isqry[a];
				ans[a] = ini[i].second;
				if (!--lefts) goto foundall;
			}
		}
		
//if(tc==22)cerr<<"hello"<<endl;
		for (; ; ) {
//for(i=0;i<lens[iq];++i){
//assert(~prev[ que[iq][i].first.first ][ que[iq][i].first.second ][ que[iq][i].second ]);
//}
			for (i = 0; i < lens[iq]; ++i) {
				if (isqry[a = que[iq][i].second]) {
					--isqry[a];
					ans[a] = que[iq][i].first;
					if (!--lefts) goto foundall;
				}
			}
			lens[ir] = 0;
			befi = befe = ~0;
			for (I = 0; I < lens[iq]; I = J) {
				for (J = I; J < lens[iq] && same[iq][I] == same[iq][J]; ++J);
				for (e = 0; e < E; ++e) for (i = I; i < J; ++i) {
					if (que[iq][i].first == p[e].second.first) {
//assert(~prev[ que[iq][i].first.first ][ que[iq][i].first.second ][ que[iq][i].second ]);
						xx = p[e].second.second.first;
						yy = p[e].second.second.second;
						b = que[iq][i].second + val[e];
						if (0 <= b && b <= LIM && !~prev[xx][yy][b]) {
//if(tc==22)if(b==110)cout<<xx<<" "<<yy<<" "<<b<<" "<<p[e].second.first.first<<" "<<p[e].second.first.second<<endl;
							prev[xx][yy][b] = e;
							same[ir][lens[ir]] = (~befi && same[iq][befi] == same[iq][i] && p[befe].first == p[e].first) ? same[ir][lens[ir] - 1] : lens[ir];
//cout<<xx<<" "<<yy<<"  "<<b<<"  "<<same[ir][lens[ir]]<<endl;
							que[ir][lens[ir]++] = mp(mp(xx, yy), b);
							befi = i; befe = e;
						}
					}
				}
			}
//cout<<endl;
			swap(iq, ir);
		}
	foundall:;
//if(tc==22)cerr<<"hello"<<endl;
		
		for (i = 0; i < Q; ++i) {
			L = 0;
			a = qrys[i];
			x = ans[a].first;
			y = ans[a].second;
			for (; (e = prev[x][y][a]) != -2; ) {
//if(tc==22)cout<<a<<" "<<x<<" "<<y<<endl;
				buf[L++] = p[e].first & 0xff;
				buf[L++] = p[e].first >> 8;
				a -= val[e];
				x = p[e].second.first.first;
				y = p[e].second.first.second;
			}
			buf[L++] = S[x][y];
			buf[L] = 0;
			reverse(buf, buf + L);
			puts(buf);
		}
		
		
	}
	
	
	return 0;
}

