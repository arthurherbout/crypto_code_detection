#define _CRT_SECURE_NO_DEPRECATE
#include <cstdio>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 128;
const int ZMAX = 15000000;

int pow3[33]={1};

int n, iter;
pair<int, int> P[MAXN];
int used[ZMAX];
vector<int> q[2];

int PX[33];

void push(int lay, int wh){
  used[wh] = iter;
  q[lay].push_back(wh);
}

int main(){
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  for (int qq=1; qq<33; qq++){
    pow3[qq] = pow3[qq-1] * 3;
  }
  int tn, t;
  scanf("%d", &tn);
  for (t=1; t<=tn; t++){
    scanf("%d", &n);
    int i, j, h, maxy=0;
    int destr[33];
    memset(PX, 0xFF, sizeof(PX));
    for (i=0; i<n; i++){
      int x, y;
      scanf("%d%d", &x, &y);
      if (y>maxy) maxy = y;
      P[i] = make_pair(x, y-1);
    }
    sort(P, P+n);
    q[0].clear();
    q[1].clear();
    push(0, 0);
    int clay = 0;
    int minq=0;
    for (i=0; i<n; i++){
      iter++;
      int cy = P[i].second;
      int bestq = 4;
      q[1-clay].clear();
      if (q[clay].size() > 3000000){
        minq = 4;
        //printf("   !!");
        break;
      }
      for (j=0; j<(int)q[clay].size(); j++){
        int pmask = q[clay][j], ppmask=pmask;
        int cnt[3]={0};
        for (h=0; h<maxy; h++){
          destr[h] = ppmask % 3;
          ppmask /= 3;
          if (PX[h]>=0) cnt[destr[h]]++;
        }
        pmask -= pow3[cy] * destr[cy];
        //if (PX[cy]>=0) cnt[destr[cy]]--;
        int f1=-1, f2=-1, f3=-1;
        if (PX[cy]>=0){
          f1=destr[cy];
        }
        if (cy>0 && PX[cy-1]>=0 && PX[cy]<PX[cy-1]){
          f2=destr[cy-1];
        }
        if (cy<maxy-1 && PX[cy+1]>=0 && PX[cy]<PX[cy+1]){
          f3=destr[cy+1];
        }
        for (h=0; h<3; h++){
          if (h==f1 || h==f2 || h==f3) continue;
          cnt[h]++;
          int zcnt = (cnt[0]>0) + (cnt[1]>0) + (cnt[2]>0);
          if (zcnt < bestq) bestq = zcnt;
          cnt[h]--;
          int nmask = pmask + pow3[cy] * h;
          if (used[nmask] != iter){
            push(1-clay, nmask);
          }
        }
      }
      if (bestq > minq) minq = bestq;
      clay=1-clay;
      PX[cy] = i;
    }
    printf("Case #%d: %d\n", t, minq);
    fflush(stdout);
  }
  return 0;
}
