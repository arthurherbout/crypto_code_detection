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

int main() {
  freopen("in.cpp","r",stdin);
  //freopen("B-large.out","w",stdout);
  int T,ncase=0;
  cin>>T;
  while(T--) {
    cin>>n>>m;
    for(int i=0; i<n; i++)cin>>c[i];
    for(int i=0; i<n; i++) {
      for(int j=0; j<m; j++)cin>>a[i][j];
      sort(a[i],a[i]+m);
    }
    vector<int>p(n,0);
    int ret=0;
    while(1) {
      bool sign=0;
      for(int i=0; i<n; i++)
        if(p[i]>=m) {
          sign=1;
          break;
        }
      if(sign)break;
      auto pr=calHelper(a[0][p[0]],c[0]);
      for(int i=1; i<n; i++)
        pr=mergeSeg(pr,calHelper(a[i][p[i]],c[i]));
      if(pr.second!=0) {
        ret++;
        for(int i=0; i<n; i++)p[i]++;
      } else {
        int mi = 1000000007,idx=0;
        for(int i=0; i<n; i++)
          if(mi>calHelper(a[i][p[i]],c[i]).second) {
            mi=calHelper(a[i][p[i]],c[i]).second;
            idx=i;
          }
        p[idx]++;
      }
    }
    cout<<"Case #"<<++ncase<<": "<<ret<<endl;
  }
  return 0;
}
