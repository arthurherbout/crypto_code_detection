#include<iostream>
#include<cstring>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

bool eq(double a, double b) {
  return fabs(a - b) < 1e-5;
}

int main() {
  int cases;
  scanf("%d", &cases);
  for(int cc = 0; cc < cases; ++cc) {
    int n; 
    double v, x;
    cin >> n >> v >> x;

    vector<double> rate(n), temp(n);
    for(int i = 0; i < n; ++i) {
      cin >> rate[i] >> temp[i];
    }

    printf("Case #%d: ", cc+1);

    if(n == 1) {
      if(temp[0] == x) {
        printf("%.10lf\n", v / rate[0]);
      }
      else {
        printf("IMPOSSIBLE\n");
      }
    }    
    else {

      double p = temp[0], q = temp[1];
      if(eq(p, q)) {
        if(eq(x, p)) {
          printf("%.10lf\n", v / (rate[0] + rate[1]));
        }
        else {
          printf("IMPOSSIBLE\n");
        }
      }
      else {
        double a = (v * x - v * q) / (p - q);
        if(a < -1e-8 || a > v * (1+1e-8)) {
          printf("IMPOSSIBLE\n");
        }
        else {
          double b = v - a;
          printf("%.10lf\n", max(a / rate[0], b / rate[1]));
        }
      }
    }
  }
}

