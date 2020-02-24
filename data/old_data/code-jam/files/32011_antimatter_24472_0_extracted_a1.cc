#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
using namespace std;

typedef long long ii;

int N;
int foo[5000][3];


int main() {
  int T;
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> N;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < 3; j++)
	cin >> foo[i][j];

    int ret = 0;
    for (int a = 0; a <= 10000; a++)
      for (int b = 0; a + b <= 10000; b++) {
	int c = 10000 - a - b;

	int ct = 0;
	for (int i = 0; i < N; i++) {
	  if (a >= foo[i][0] && b >= foo[i][1] && c >= foo[i][2])
	    ct++;
	}
	if (ct > ret) ret = ct;
      }

    printf("Case #%d: %d\n", it, ret);
  }
}
