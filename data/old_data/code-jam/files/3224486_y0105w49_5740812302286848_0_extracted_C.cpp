#include <bits/stdc++.h>
using namespace std;

typedef long double ld;

struct As {
  ld x,y,z,vx,vy,vz;
  ld distto(As &o) {
    ld xx=x-o.x;
    ld yy=y-o.y;
    ld zz=z-o.z;
    return sqrt(xx*xx+yy*yy+zz*zz);
  }
} as[1<<10];
int N,S;

ld dist[1<<10][1<<10];
int seen[1<<10];
bool dfs(int u,ld M) {
  if(u==1) return true;
  if(seen[u]) return false;
  seen[u]=true;
  for(int i=0;i<N;i++) if(dist[u][i]<=M && dfs(i,M)) return true;
  return false;
}


int main() {
  int t; cin>>t; for(int zz=1;zz<=t;zz++) {
    cin>>N>>S;
    for(int i=0;i<N;i++) cin>>as[i].x>>as[i].y>>as[i].z>>as[i].vx>>as[i].vy>>as[i].vz;
    for(int i=0;i<N;i++) for(int j=i;j<N;j++) {
        dist[i][j]=dist[j][i]=as[i].distto(as[j]);
      }

    ld L=0,R=3000;
    for(int uft=50; --uft;) {
      ld M=(L+R)/2;
      memset(seen,0,sizeof seen);
      if(dfs(0,M)) R=M;
      else L=M;
    }


    printf("Case #%d: ",zz); cout<<fixed<<setprecision(10)<<(L+R)/2<<endl;
  }
}
