#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cstring>
#include <queue>
#include <map>
using namespace std;
typedef long long ll;
const int maxn = 55;

int n,m,a[maxn][maxn],c[maxn];

pair<int,int>calHelper(int x,int c) {
  int r=x*10/(9*c);
  int l=(x*10+11*c-1)/(11*c);
  if(l<=r)return make_pair(l,r);
  return make_pair(0,0);
}
pair<int,int>mergeSeg(pair<int,int>a,pair<int,int>b) {
  int l=max(a.first,b.first);
  int r=min(a.second,b.second);
  if(l<=r)return make_pair(l,r);
  return make_pair(0,0);
}

bool used[maxn][maxn];
int main() {
  freopen("B-small-attempt0.in","r",stdin);
  freopen("B-small.out","w",stdout);
  int T,ncase=0;
  cin>>T;
  while(T--) {
    cin>>n>>m;
    for(int i=0; i<n; i++)cin>>c[i];
    for(int i=0; i<n; i++) {
      for(int j=0; j<m; j++)cin>>a[i][j];
      sort(a[i],a[i]+m);
    }
    memset(used,0,sizeof(used));
    int ret=0;
    for(int j=m-1; j>=0; j--) {
      pair<int,int>ans=calHelper(a[0][j],c[0]);
      if(ans.second==0)continue;
      bool fail=false;
      vector<int>path(n,-1);
      for(int i=1; i<n; i++) {
        bool isFind=false;
        for(int k=m-1; k>=0; k--) {
          if(used[i][k])continue;
          auto pr=calHelper(a[i][k],c[i]);
          auto mg=mergeSeg(ans,pr);
          if(mg.second!=0) {
            isFind=true;
            ans=mg;
            path[i]=k;
            break;
          }
        }
        if(!isFind) {
          fail=true;
          break;
        }
      }
      if(!fail) {
        ret++;
        for(int i=1; i<n; i++)used[i][path[i]]=true;
      }
    }
    cout<<"Case #"<<++ncase<<": "<<ret<<endl;
  }
  return 0;
}
