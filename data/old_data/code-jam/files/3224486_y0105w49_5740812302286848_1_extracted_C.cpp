#include <bits/stdc++.h>
using namespace std;

typedef long double ld;

#define eps 1e-9
//#define eps 0

ld M,T;
int N,S;
struct As {
  ld x,y,z,vx,vy,vz;
  ld distto(As &o) {
    ld xx=x-o.x;
    ld yy=y-o.y;
    ld zz=z-o.z;
    return sqrt(xx*xx+yy*yy+zz*zz);
  }
} as[1<<10];

bool crosses[1<<10][1<<10];
ld t1[1<<10][1<<10],t2[1<<10][1<<10];

void gen(int i,int j) {
  ld x=as[j].x-as[i].x;
  ld y=as[j].y-as[i].y;
  ld z=as[j].z-as[i].z;
  ld vx=as[j].vx-as[i].vx;
  ld vy=as[j].vy-as[i].vy;
  ld vz=as[j].vz-as[i].vz;
  ld a=vx*vx+vy*vy+vz*vz;
  ld b=2*(vx*x+vy*y+vz*z);
  ld c=x*x+y*y+z*z-M*M;
  ld d=b*b-4*a*c;

  //  if(i==0 && j==2) cout<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<" "<<c<<" "<<d<<endl;

  bool qq;
  if(vx==0&&vy==0&&vz==0) {
    crosses[i][j]=crosses[j][i]=qq=(c<eps);
    //if(i==0 && j==2) cout<<qq<<endl;
    t1[i][j]=t1[j][i]=-1;
    t2[i][j]=t2[j][i]=-2;
    if(qq) t2[i][j]=t2[j][i]=3e7;
    return;
  }


  crosses[i][j]=crosses[j][i]=qq=(d>-eps);
  if(!qq) {
    t1[i][j]=t1[j][i]=-1;
    t2[i][j]=t2[j][i]=-2;
    return;
  }
  if(d<0) d=0;
  t1[i][j]=t1[j][i]=(-b-sqrt(d))/(2*a);
  t2[i][j]=t2[j][i]=(-b+sqrt(d))/(2*a);
}

vector<pair<ld,pair<int,int> > > acts;
bool red[1<<10];

bool dfs1(int u) {
  if(u==1) return true;
  if(red[u]) return false;
  red[u]=true;
  for(int v=0;v<N;v++) if(crosses[u][v] && t1[u][v]<T+eps && t2[u][v]>T-eps && dfs1(v)) return true;
  return false;
}

int main() {
  cout<<fixed<<setprecision(10);
  int t; cin>>t; for(int zz=1;zz<=t;zz++) {
    acts.clear();
    cin>>N>>S;
    for(int i=0;i<N;i++) cin>>as[i].x>>as[i].y>>as[i].z>>as[i].vx>>as[i].vy>>as[i].vz;

    ld L=0,R=8000;
    for(int uft=50; --uft;) {
      M=(L+R)/2;
      for(int i=0;i<N;i++) for(int j=i;j<N;j++) gen(i,j);
      acts.clear();
      for(int i=0;i<N;i++) {
        vector<pair<ld,char> > lacts;
        for(int j=0;j<N;j++) {
          if(j==i) continue;
          if(crosses[i][j]) {
            lacts.emplace_back(t1[i][j],'B');
            lacts.emplace_back(t2[i][j],'E');
            acts.emplace_back(t1[i][j],make_pair(i,j));
          }
        }
        lacts.emplace_back(0,'B');
        lacts.emplace_back(0,'E');
        sort(lacts.begin(),lacts.end());
        int ct=0;
        for(int k=0;k<lacts.size();k++) {
          auto act=lacts[k];
          if(act.second=='B') ++ct;
          else if(act.second=='E') --ct;
          else assert(0);

          if(!ct && (k+1==lacts.size() || lacts[k+1].first > act.first+S+eps)) acts.emplace_back(act.first+S,make_pair(i,-1));
        }
      }
      sort(acts.begin(),acts.end());

      /*
      cout<<M<<endl;
      for(auto act:acts) cout<<act.first<<" "<<act.second.first<<" "<<act.second.second<<endl;
      cout<<endl; */

      memset(red,0,sizeof red);
      T=0;
      if(dfs1(0)) goto gg;

      for(auto act:acts) {
        T=act.first;
        if(T<-eps) continue;
        int u=act.second.first, v=act.second.second;
        if(v==-1) red[u]=0;
        else if((red[u]||red[v]) && (dfs1(u)||dfs1(v))) goto gg;
      }


      L=M; continue;
    gg: R=M;
    }


    printf("Case #%d: ",zz); cout<<(L+R)/2<<endl;
  }
}
