#include <cstdio>
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

double put(double a, double v0, double t){
  return v0*t + a*t*t/2.0;
}

double t[100], x[100], v[100];

double koliko(double a, double D){
  double stigao = (D-x[0])/v[0];

  double a2 = 2*D/(stigao*stigao);
  if(a2 <= a) return stigao;

  return sqrt(2.0*D/a);
}

void solve(){
  double D;
  int N, A;

  cin >> D >> N >> A;
  for(int i = 0; i < N; i++)
    cin >> t[i] >> x[i];
  for(int i = 0; i < N-1; i++)
    v[i] = (x[i+1]-x[i])/(t[i+1]-t[i]);

  for(int i = 0; i < A; i++){
    double a;
    cin >> a;

    printf( "%.6lf\n", koliko(a, D));
  }
}

int main(){
  int t;
  cin >> t;

  for(int i = 0; i < t; i++){
    printf("Case #%d: \n", i+1);
    solve();
  }
  return 0;
}
