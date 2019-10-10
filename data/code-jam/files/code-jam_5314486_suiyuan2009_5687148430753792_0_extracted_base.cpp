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
const int maxn = 1005;

bool seat_full[maxn][maxn],id_in[maxn][maxn];
int num[maxn];
int n,m,c;
struct P {
  int id,seat;
  P() {}
  P(int id,int seat):id(id),seat(seat) {}
  bool operator<(const P&a)const {
    return seat<a.seat;
  }
} p[maxn];

int solve_1() {
  memset(num,0,sizeof(num));
  memset(id_in,0,sizeof(id_in));
  int ret=0;
  for(int i=0; i<m; i++) {
    bool sign=0;
    for(int j=0; j<ret; j++)
      if((!id_in[j][p[i].id])&&num[j]<p[i].seat) {
        sign=1;
        num[j]++;
        id_in[j][p[i].id]=1;
        break;
      }
    if(!sign) {
      num[ret]++;
      id_in[ret][p[i].id]=1;
      ret++;
    }
  }
  return ret;
}
int solve_2(int cnt) {
  memset(id_in,0,sizeof(id_in));
  memset(seat_full,0,sizeof(seat_full));
  int ret=0;
  for(int i=0; i<m; i++) {
    bool sign=0;
    for(int j=0; j<cnt; j++) {
      if((!id_in[j][p[i].id])&&(!seat_full[j][p[i].seat])) {
        sign=1;
        id_in[j][p[i].id]=1;
        seat_full[j][p[i].seat]=1;
        break;
      }
    }
    if(!sign)ret++;
  }
  return ret;
}

int main() {
  //freopen("in.cpp","r",stdin);
  freopen("B-small-attempt0.in","r",stdin);
  freopen("B-small.out","w",stdout);
  int T;
  cin>>T;
  int ncase=0;
  while(T--) {
    cin>>n>>c>>m;
    for(int i=0; i<m; i++) {
      int x,y;
      cin>>x>>y;
      p[i]=P(y,x);
    }
    sort(p,p+m);
    int cnt=solve_1();
    cout<<"Case #"<<++ncase<<": "<<cnt<<" "<<solve_2(cnt)<<endl;
  }
  return 0;
}
