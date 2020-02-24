
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <complex>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iomanip>
using namespace std;

double data[6][1<<5];

int main(void){
  int cases;
  cin >> cases;
  for(int case_no = 1; case_no <= cases; case_no++){
    
    int n, start;
    double p;
    cin >> n >> p >> start;
    int T = 1<<5;
    for(int i=0; i< T; i++)
      data[0][i] = 0.0;
    for(int i=1; i<=5; i++){
      for(int j=0; j<T; j++){
	double s = 0.0;
	for(int k=0; k<=j; k++){
	  double s2 = (1-p) * data[i-1][j-k] + p * (j+k >= T ? 1.0 : data[i-1][j+k]);
	  s = max(s, s2);
	}
	data[i][j] = s;
      }
    }
    int idx = start * T / 1000000;
    double ans = 1.0;
    if(idx != T)
      ans = data[n][idx];
    cout << "Case #" << case_no << ": " << fixed << setprecision(10) << ans << endl;
  }
}
