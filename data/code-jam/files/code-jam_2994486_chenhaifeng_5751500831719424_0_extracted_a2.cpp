//============================================================================
// Name        : asmall.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

int min(int s1, int s2, int s3, int s4) {
	vector<int> v;
	v.push_back(s1);
	v.push_back(s2);
	v.push_back(s3);
	v.push_back(s4);
	sort(v.begin(), v.end());
	return v[0];
}

int main() {
	freopen("A-small-attempt4.in", "r", stdin);
	freopen("A-small-attempt4.out", "w", stdout);
	int T, N;
	string x, y;
	char chr[105];
	scanf("%d", &T);
	for (int t = 0; t < T; t++) {
		printf("Case #%d: ", t + 1);
		scanf("%d", &N);
		scanf("%s", chr);
		x.clear();
		x = chr;
		scanf("%s", chr);
		y.clear();
		y = chr;

		vector<string> xv;
		vector<string> yv;

		string xx;
		xx = x[0];
		int j = 0;
		string tmpx;
		tmpx = x[0];
		for (size_t i = 1; i < x.size(); i++) {
			if (x[i] == xx[j]) {
				tmpx += x[i];
				continue;
			} else {
				xv.push_back(tmpx);
				tmpx.clear();
				tmpx = x[i];
				xx += x[i];
				j++;
			}
		}
		if (tmpx.size() != 0)
			;
		xv.push_back(tmpx);

		string yy;
		yy = y[0];
		j = 0;
		string tmpy;
		tmpy = y[0];
		for (size_t i = 1; i < y.size(); i++) {
			if (y[i] == yy[j]) {
				tmpy += y[i];
				continue;
			} else {
				yv.push_back(tmpy);
				tmpy.clear();
				tmpy = y[i];
				yy += y[i];
				j++;
			}
		}
		if (tmpy.size() != 0)
			;
		yv.push_back(tmpy);

		if (xx == yy) {
			//cout << xx << endl;
			//cout << yy << endl;
			int ans = 0;
			if (xv.size() != yv.size())
				printf("-------------------------------------------wrong\n");
			for (size_t i = 0; i < xv.size(); i++) {
				int l1 = xv[i].size();
				int l2 = yv[i].size();
				if (l1 < l2) {
					ans += (l2 - l1);
				} else {
					ans += (l1 - l2);
				}
			}
			printf("%d\n", ans);
		} else {
			printf("Fegla Won\n");
		}
	}
	return 0;

}
