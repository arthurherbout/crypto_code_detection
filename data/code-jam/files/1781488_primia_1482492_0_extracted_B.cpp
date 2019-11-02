#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;
const double eps = 1e-12;
int main()
{
  int T;
  cin >> T;
  
  for(int tc=1;tc<=T;++tc){
    double D;
    int N,A;
    cin >> D >> N >> A;

    double t[N];
    double x[N];
    double a[A];

    cout <<"Case #" <<tc<<":"<<endl;
    for(int i = 0; i < N; ++i){
      cin >> t[i] >> x[i];
    }
    
    for(int i = 0; i < A; ++i){
      cin >> a[i];

      double time = sqrt((2*D)/a[i]);
      if( N == 2 ){
        double v = (x[1]-x[0])/(t[1]-t[0]);
        double tt;
        //if( v*v+2*a[i]*x[1] < 0 ){
        //tt = 1e50;
        //}else{
          tt = (v+sqrt(v*v+2*a[i]*x[0]))/a[i];
          //}
        //2*v/a[i];
  
          //cout << x[0] + v*tt << ' ' << D << endl;
        if( x[0] + v*tt < D ){
          time = tt+(D-(x[0]+v*tt))/v;
        }
      }
      cout << fixed << setprecision(8) << time << endl;
    }
  }
  return 0;
}
