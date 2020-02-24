#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cstring>
#include <queue>
#include <map>
#include <set>
#include <stack>
using namespace std;
typedef long long ll;
const int maxn = 105;

int f2[maxn][2];
int f3[maxn][maxn][3];
int f4[maxn][maxn][maxn][4];
int n,m,a[maxn];

int c[4];
int solve() {
  memset(c,0,sizeof(c));
  for(int i=0; i<n; i++)
    c[a[i]%m]++;
  int ret=c[0];
  int ans=0;
  if(m==2) {
    memset(f2,-1,sizeof(f2));
    f2[0][0]=0;
    for(int i=0; i<=c[1]; i++)
      for(int j=0; j<2; j++) {
        if(f2[i][j]==-1)continue;
        if(i<c[1])f2[i+1][(j+1)%m]=max(f2[i+1][(j+1)%m],f2[i][j]+(j==0));
        ans=max(ans,f2[i][j]);
      }
    ret+=ans;
  } else if(m==3) {
    memset(f3,-1,sizeof(f3));
    f3[0][0][0]=0;
    for(int i=0; i<=c[1]; i++)
      for(int j=0; j<=c[2]; j++)
        for(int k=0; k<3; k++) {
          if(f3[i][j][k]==-1)continue;
          if(i<c[1])f3[i+1][j][(k+1)%m]=max(f3[i+1][j][(k+1)%m],f3[i][j][k]+(k==0));
          if(j<c[2])f3[i][j+1][(k+2)%m]=max(f3[i][j+1][(k+2)%m],f3[i][j][k]+(k==0));
          ans=max(ans,f3[i][j][k]);
        }
    ret+=ans;
  } else {
    memset(f4,-1,sizeof(f4));
    f4[0][0][0][0]=0;
    for(int i=0; i<=c[1]; i++)
      for(int j=0; j<=c[2]; j++)
        for(int k=0; k<=c[3]; k++)
          for(int r=0; r<4; r++) {
            if(f4[i][j][k][r]==-1)continue;
            if(i<c[1])f4[i+1][j][k][(r+1)%m]=max(f4[i+1][j][k][(r+1)%m],f4[i][j][k][r]+(r==0));
            if(j<c[2])f4[i][j+1][k][(r+2)%m]=max(f4[i][j+1][k][(r+2)%m],f4[i][j][k][r]+(r==0));
            if(k<c[3])f4[i][j][k+1][(r+3)%m]=max(f4[i][j][k+1][(r+3)%m],f4[i][j][k][r]+(r==0));
            ans=max(ans,f4[i][j][k][r]);
          }
    ret+=ans;
  }
  return ret;
}

int main() {
  //freopen("in.cpp","r",stdin);
  freopen("A-large.in","r",stdin);
  freopen("A-large.out","w",stdout);
  int T;
  cin>>T;
  int ncase=0;
  while(T--) {
    cin>>n>>m;
    for(int i=0; i<n; i++)cin>>a[i];
    int ret=solve();
    cout<<"Case #"<<++ncase<<": "<<ret<<endl;
  }
  return 0;
}
