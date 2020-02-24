//============================================================================
// Name        : bsmall.cpp
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
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("out.txt", "w", stdout);
	int T;
	int A, B, C;
	scanf("%d", &T);
	for(int t = 0;t < T;t++) {
		printf("Case #%d: ", t + 1);
		scanf("%d%d%d", &A, &B, &C);

		int cnt = 0;
		for(int i = 0;i < A;i++) {
			for(int j = 0;j < B;j++) {
				if((i & j) < C) cnt++;
			}
		}
		printf("%d\n", cnt);
	}

	return 0;
}
