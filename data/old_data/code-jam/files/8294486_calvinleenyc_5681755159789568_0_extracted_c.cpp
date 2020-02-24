#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
const int maxn = 105;
ll e[maxn];
ll s[maxn];
ll d[maxn][maxn];
double t[maxn][maxn];
const ll inf = 1e15;

void solve(){
  int n; cin >> n;
  int q; cin >> q;
  for(int i=0;i<n;i++){
    cin >> e[i] >> s[i];
  }
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      cin >> d[i][j];
      if(d[i][j] == -1) d[i][j] = inf;
    }
  }
  for(int k=0;k<n;k++){
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
	d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
      }
    }
  }
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      if(d[i][j] <= e[i]){
	t[i][j] = d[i][j] * 1.0 / s[i];
      }
      else{
	t[i][j] = inf;
      }
    }
  }
  
  for(int k=0;k<n;k++){
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
	t[i][j] = min(t[i][j], t[i][k] + t[k][j]);
      }
    }
  }
  for(int qq=0;qq<q;qq++){
    int i,j; cin >> i >> j; i--; j--;
    printf("%.10f ", t[i][j]);
  }
}

int main(){
  int T; cin >> T;
  for(int i=0;i<T;i++){
    cout << "Case #" << (i+1) << ": "; solve(); cout << "\n";
  }
}
