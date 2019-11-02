#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
#include <cmath>
using namespace std;

typedef long long ii;

double p[30][4];
vector<double> allp;
int Q;

void go(int idx, double P) {
  if (idx == Q) {
    allp.push_back(P);
  }
  for (int i = 0; i < 4; i++) {
    if (p[idx][i] > 0) {
      go(idx+1, P*p[idx][i]);
    }
  }
}

int main() {
  int C; cin >> C;
  int M;
  for (int it = 1; it <= C; it++) {
    cin >> M >> Q;

    for (int i = 0; i < Q; i++)
      for (int j = 0; j < 4; j++)
	cin >> p[i][j];

    allp.clear();
    go(0, 1);
    sort(allp.rbegin(), allp.rend());

    double ret = 0;
    for (int i = 0; i < M && i < allp.size(); i++) {
      ret += allp[i];
    }

    printf("Case #%d: %.6lf\n", it, ret);
  }
}
