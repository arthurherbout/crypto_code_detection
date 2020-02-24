#include <stdio.h>
#include <queue>
using namespace std;

#define mp make_pair
#define PII pair<int, int>

int dp[105][205][105][105];
queue< pair< pair< PII, PII >, int> > q;

int main(){
  int T;
  scanf("%d", &T);
  for(int tc=1; tc<=T; tc++){
    int Hd, Ad, Hk, Ak, B, D;
    scanf("%d%d%d%d%d%d", &Hd, &Ad, &Hk, &Ak, &B, &D);
    while(!q.empty()) q.pop();
    for(int i=0; i<105; i++) for(int j=0; j<205; j++) for(int k=0; k<105; k++) for(int l=0; l<105; l++) dp[i][j][k][l] = 1e9;
    q.push(mp(mp(mp(Hd,Ad),mp(Hk,Ak)), 0));
    printf("Case #%d: ", tc);
    int found = 0;
    while(!q.empty()){
      int hd = q.front().first.first.first;
      int ad = q.front().first.first.second;
      int hk = q.front().first.second.first;
      int ak = q.front().first.second.second;
      int dist = q.front().second;
      q.pop();
      //printf("vs: %d %d %d %d\n", hd, ad, hk, ak);
      if(hk == 0){ printf("%d\n", dist); found = 1; break; }
      if(hd == 0) continue;
      if(dp[hd][ad][hk][ak] != 1e9) continue;
      dp[hd][ad][hk][ak] = dist;
      int newhealth = max(hd-ak, 0);
      //attack
      q.push(mp(mp(mp(newhealth, ad), mp(max(hk-ad, 0), ak)), dist+1));
      //buff
      if(ad < hk) q.push(mp(mp(mp(newhealth, ad+B), mp(hk, ak)), dist+1));
      //cure
      q.push(mp(mp(mp(max(Hd-ak, 0), ad), mp(hk, ak)), dist+1));
      //debuff
      int newatk = max(ak-D, 0);
      q.push(mp(mp(mp(max(hd-newatk, 0), ad), mp(hk, newatk)), dist+1));
    }
    if(found == 0) printf("IMPOSSIBLE\n");
  }
}
