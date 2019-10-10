// jam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
#include <utility>
#include <cctype>
#include <vector>
#include <queue>

using namespace std;

int n, sub;
vector<int> b(40);

vector<double> a[1000], pp;

void dfs(int k,double p) {
	int i;
	if (k == n) {
		pp.push_back(p);
		return;
	}
	for (i = 0; i < 4; i++) {
		dfs(k+1,p*a[k][i]);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int zz, testcases;

	freopen("p32.in","r",stdin);
	freopen("p3.out","w",stdout);

	scanf("%d", &testcases);
	int i, j, k, opt;
	double p, tp, mp, tot, rest, t;

	for (zz = 1; zz <= testcases; zz++) {
		pp.clear();
		printf("Case #%d: ",zz);
		scanf("%d%d",&sub,&n);
		for (i = 0; i < n; i++) {
			vector<double> tmp;
			for (j = 0; j < 4; j++) {
				double t;
				scanf("%lf",&t);
				tmp.push_back(t);
			}
			sort(tmp.begin(),tmp.end());
			reverse(tmp.begin(), tmp.end());
			a[i] = tmp;
		}
		if (sub >= (1LL << (2 * n))) {
			printf("1\n");
			continue;
		}
		dfs(0,1);
		sort(pp.begin(),pp.end());
		reverse(pp.begin(),pp.end());
		/*
		p = 1;
		for (i = 0; i < n; i++) {
			b[i] = 0;
			p *= a[i][0];
		}
		tot = 0;
		rest = 1;
		pp = qq = 0;
		for (i = 0; i < sub; i++) {
			b = queue[pp];
			tot += rest * p;
			rest -= tot;
			mp = -1;
			opt = -1;
			for (j = 0; j < n; j++) if (b[j] < 3) {
				b[j]++;
				tp = 1;
				for (k = 0; k < n; k++) {
					tp *= a[k][b[k]];
				}
				b[j]--;
				if (tp > mp) {
					mp = tp;
					opt = j;
				}
			}
			if (opt == -1) {
				printf("!!!\n");
				exit(0);
			}
			b[opt]++;
			p = mp;
			pp++;
		}
		*/
		tot = 0;
		rest = 1;
		if (sub > pp.size()) {
			printf("!!!\n");
			exit(0);
		}
		for (i = 0; i < sub; i++) {
//			printf("%lf\n",pp[i]);
//			tot += rest * pp[i];
//			rest -= tot;
			tot += pp[i];
		}
		printf("%.10lf\n",tot);
	}

	return 0;
}
