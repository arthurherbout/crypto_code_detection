#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

int main(){
  int n,d,ins,m;
  int t;
  cin >> t;
  for(int id=1; id<=t; id++){
    cout << "Case #" << id << ": ";
    cin >> d >> ins >> m >> n;
    vector<int> tab(n);
    for(int i=0; i<n; i++)cin >>tab[i];

    vector< vector<int> > dp(n,vector<int>(256,(1<<29)));

    for(int i=0; i<256; i++){
      dp[0][i] = abs(tab[0]-i);
    }
    if(n>1){
      dp[0][tab[1]] = min(dp[0][tab[1]],d);
    }

    for(int i=0; i<n-1; i++){
      for(int j=0; j<256; j++){
	if(dp[i][j] == (1<<29))continue;
	int pos = j; int npos = tab[i+1];
	int ab = abs(pos-npos);
	if(m!=0){
	  int nc = ((ab/m))*ins;
	  if(ab%m==0)nc -= ins;
	  nc = max(0,nc);
	  nc += dp[i][pos];
	  dp[i+1][npos] = min(dp[i+1][npos],nc);
	}
	dp[i+1][pos] = min(dp[i+1][pos],dp[i][pos]+d);
	for(int pl=0; pl <= m; pl++){
	  int upb = pos + pl;
	  int lwb = pos -pl;
	  if(upb < 256){
	    int nc = abs(upb-npos);
	    dp[i+1][upb] = min(dp[i+1][upb],dp[i][pos]+nc);
	  }
	  if(lwb >= 0){
	    int nc = abs(lwb-npos);
	    dp[i+1][lwb] = min(dp[i+1][lwb],dp[i][pos]+nc);
	  }
	}
      }
    }
    int ans = (1<<29);
    for(int i=0; i<256; i++)ans = min(dp[n-1][i],ans);
    cout << ans << endl;
  }
  return 0;
}
