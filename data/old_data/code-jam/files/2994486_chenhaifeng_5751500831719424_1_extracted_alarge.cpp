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

int main() {
	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);
	int T, N;
	string x, y;
	char chr[105];
	vector<string> vStr;
	scanf("%d", &T);
	for (int t = 0; t < T; t++) {
		printf("Case #%d: ", t + 1);
		scanf("%d", &N);
		vStr.clear();
		for (int i = 0; i < N; i++) {
			scanf("%s", chr);
			vStr.push_back(chr);
		}
		vector<string> identical;
		vector<vector<int> > len(N);
		for (int j = 0; j < N; j++) {
			string xx;
			xx = vStr[j][0];
			int l = 1;
			int k = 0;
			for (size_t i = 1; i < vStr[j].size(); i++) {
				if (vStr[j][i] == xx[k]) {
					l++;
				} else {
					len[j].push_back(l);
					l = 1;
					xx += vStr[j][i];
					k++;
				}
			}
			if (l != 0)
				len[j].push_back(l); //注意这里
			identical.push_back(xx);
		}
		int finish = 0;
		for (int i = 1; i < N; i++) {
			if (identical[i] != identical[0]) {
				printf("Fegla Won\n");
				finish = 1;
				break;
			}
		}
		if (finish == 1)
			continue;
		int ans = 0;
		for (size_t i = 0; i < len[0].size(); i++) {
			int sum = 0;
			for (int j = 0; j < N; j++) {
				sum += len[j][i];
			}
			int bio;
			if (double(sum) / N - sum / N > 0.5)
				bio = sum / N + 1;
			else
				bio = sum / N;
			for (int j = 0; j < N; j++) {
				ans += (len[j][i] > bio ? len[j][i] - bio : bio - len[j][i]);
			}
		}
		printf("%d\n", ans);
	}
	return 0;

}
