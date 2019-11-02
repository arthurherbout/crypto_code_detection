/*
	Author       :	Jan
	Problem Name :
	Algorithm    :
	Complexity   :
*/

#include <set>
#include <map>
#include <list>
#include <cmath>
#include <ctime>
#include <queue>
#include <stack>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

int cases, caseno, n;

struct info {
	int c, s, t;
	bool operator < ( const info &b ) const {
		if( t != b.t ) return t < b.t;
		if( c != b.c ) return c < b.c;
		return s < b.s;
	}
}a[100], b[100];

int main() {
	freopen("c.in", "r", stdin);
	freopen("c.ans", "w", stdout);

	double cl = clock();
	scanf("%d", &cases);
	while( cases-- ) {
		printf("Case #%d: ", ++caseno);

		priority_queue < info, vector <info> > Q;
		scanf("%d", &n);
		for( int i = 0; i < n; i++ ) {
			info x;
			scanf("%d %d %d", &x.c, &x.s, &x.t);
			Q.push(x);
		}
		int m;
		scanf("%d", &m);
		for( int i = 0; i < m; i++ ) scanf("%d %d %d", &a[i].c, &a[i].s, &a[i].t);
		int t = 1, p = 0, s = 0, res = 0;
		while( !Q.empty() && t ) {
			info x = Q.top();

			//printf("%d %d %d\n", x.c, x.s, x.t);

			if( x.t > 0 ) {
				Q.pop();
				s += x.s;
				t = t - 1 + x.t;
				if( x.c && p < m ) {
					Q.push( a[p] );
					p++;
				}
				res = max( s, res );

			}
			else {
				int y = 0, score[100];
				while( !Q.empty() ) {
					b[y] = Q.top();
					score[y++] = b[y].s;
					Q.pop();
				}
				sort( score, score + y, greater <int> () );
				int s1 = 0;
				for( int j = 0; j < y && j < t; j++ ) s1 += score[j];
				res = max( res, s1 + s );

				for( int j = 0; j < y; j++ ) Q.push(b[j]);

				if( !Q.empty() ) {
					x = Q.top();
					Q.pop();
					s += x.s;
					if( x.c && p < m ) {
						Q.push( a[p] );
						p++;
					}
					res = max( s, res );
				}
				t--;
			}
		}
		printf("%d\n", res);
	}

	cl = clock() - cl;
	fprintf(stderr, "Total Execution Time = %lf seconds\n", cl / CLOCKS_PER_SEC);

	return 0;
}
