#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <complex>

using namespace std;

typedef long long Int;
template<typename T> void chmax(T& a, T b) { if (a < b) a = b; }
template<typename T> void chmin(T& a, T b) { if (a > b) a = b; }

Int N, X, C;
Int S[1024], E[1024], T[1024];

map<pair<Int, Int>, int> back;

Int calc(Int t, Int p)
{
	Int leave = X;
	
	for (int i = 0; i < C; ++i) {
		if (T[i] + E[i] - S[i] < t) {
			continue;
		}
		if (T[i] + E[i] - S[i] == t) {
			if (E[i] % N == p) {
				chmin(leave, t);
			}
			continue;
		}
		
		Int pos, ppos, cur;
		if (T[i] <= t) {
			pos = (S[i] + ((t - T[i]) % N + N) % N) % N;
			ppos = p;
			cur = t;
		} else {
			pos = S[i];
			ppos = (p - (T[i] - t) % N + 2 * N) % N;
			cur = T[i];
		}
		
		if (pos == ppos) {
			chmin(leave, cur - 1);
			continue;
		}

/*
		while (true) {
			if (pos == E[i] % N) {
				cur = X;
				break;
			}
			if ((pos + 1) % N == ppos) break;
			if ((pos + 1) % N == (ppos - 1 + N) % N) break;
			pos = (pos + 1) % N;
			ppos = (ppos - 1 + N) % N;
			++cur;
		}
		
		for (int j = 0; j < C; ++j) {
			if (S[j] == ppos && T[j] == cur) {
				--cur;
				j = -1;
				ppos = (ppos + 1 + N) % N;
			}
		}
		
		chmin(leave, cur);
*/
		Int dist = (ppos - pos + 2 * N) % N;
		
		if ((dist + 1) / 2 <= (E[i] % N - pos + 2 * N) % N) {
			Int stop = (dist - 1) / 2;
			cur += stop;
			ppos = (ppos - stop) % N;
			ppos = (ppos + N) % N;
			if (back.find(make_pair(cur, ppos)) != back.end()) {
				cur += back[make_pair(cur, ppos)];
				ppos = (ppos + back[make_pair(cur, ppos)] + N) % N;
			}
			chmin(leave, cur + stop);
		}
	}
	
	return leave - t;
}

Int doit(Int t, Int p)
{
	Int baset = calc(t, p);
	
	for (int i = 0; i < C; ++i) {
		if (T[i] <= t && t <= T[i] + E[i] - S[i]) {
			Int pp = (S[i] + t - T[i]) % N;
			if (p == pp) return 0;
		}
	}
	
	Int lo = 0, hi = t + 1;
	while (hi - lo > 1) {
		Int mid = (hi + lo) / 2;
		Int tmp = calc(t - mid, (p + mid) % N);
		
		if (tmp == mid + baset) {
			lo = mid;
		} else {
			hi = mid;
		}
	}
	
	return lo + baset;
}

int CC=0;
void solve()
{
	back.clear();
    cin >> C >> X >> N;
    
    for (int i = 0; i < C; ++i) {
    	cin >> S[i] >> E[i] >> T[i];
    	--S[i]; --E[i];
    	if (S[i] > E[i]) E[i] += N;
    }
    
    for (int i = 0; i < C; ++i) {
    	back[make_pair(T[i], S[i])] = 1;
    }
    for (int i = 0; i < C; ++i) {
    	for (int j = 0; j < C; ++j) {
    		Int tt = T[i], ss = S[i];
    		--tt;
    		ss = (ss+1) % N;
    		if (back.find(make_pair(tt, ss)) != back.end()) {
    			back[make_pair(T[i], S[i])] = back[make_pair(tt, ss)] + 1;
    		}
    	}
    }
    
    Int res = 0;
    for (int i = 0; i < C; ++i) {
    	for (int d = 1; d <= 2; ++d) {
	    	chmax(res, doit(T[i], (S[i] - d + N) % N));
	    	chmax(res, doit(T[i] + 1, (S[i] - (d - 1) + N) % N));
    		chmax(res, doit(T[i] + E[i] - S[i], (E[i] + d) % N));
    	}
    }
    if (C == 0) res = X;
    
    cout << res << endl;
}

int main()
{
    int CASES;
    cin >> CASES;
    
    for (int CASE = 1; CASE <= CASES; ++CASE) {
        cout << "Case #" << CASE << ": ";
        solve();   
    }
    
    return 0;
}


