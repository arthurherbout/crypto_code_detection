#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int, int> joft;
int r, c;
string tbl[50];
joft beam[2500];
map<joft, int> ind;
const int mv[4][2]={{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
typedef pair<joft, int> djoft;
bool inside(joft x){
  if (x.first<0 || x.first>=r)
    return false;
  if (x.second<0 || x.second>=c)
    return false;
  if (tbl[x.first][x.second]=='#')
    return false;
  return true;
}
djoft next(djoft x){
  if (tbl[x.first.first][x.first.second]=='/')
    x.second ^= 3;
  else if (tbl[x.first.first][x.first.second]=='\\')
    x.second ^= 1;
  x.first.first += mv[x.second][0];
  x.first.second += mv[x.second][1];
  return x;
}
int B;
map<joft, vector<int> > choice;
vector<int> ngb[5000];
vector<int> rngb[5000];
void clause(int a, int b){
  ngb[(a+B)%(B+B)].push_back(b);
  rngb[b].push_back((a+B)%(B+B));
  ngb[(b+B)%(B+B)].push_back(a);
  rngb[a].push_back((b+B)%(B+B));
}
int marked[5000];
vector<int> topol;
void dfs(int v){
  if (marked[v]==-2)
    return;
  marked[v]=-2;
  for (int i=0;i<ngb[v].size();++i)
    dfs(ngb[v][i]);
  topol.push_back(v);
}
void rdfs(int v, int comp=0){
  if (marked[v]>=0)
    return;
  marked[v]=comp;
  for (int i=0;i<rngb[v].size();++i)
    rdfs(rngb[v][i], comp);
}
int main(){
  int tcou;cin>>tcou;
  for (int ti=0;ti<tcou;++ti){
    cin>>r>>c;
    ind.clear();
    choice.clear();
    for (int i=0;i<r;++i){
      cin>>tbl[i];
      for (int j=0;j<c;++j){
        if (tbl[i][j]=='|' || tbl[i][j]=='-'){
          tbl[i][j]='-';
          int sz = ind.size();
          beam[sz] = joft(i, j);
          ind[joft(i, j)] = sz;
        }
      }
    }
    topol.clear();
    B = ind.size();
    for (int i=0;i<B+B;++i){
      ngb[i].clear();
      rngb[i].clear();
      marked[i] = -1;
    }
    for (int i=0;i<B;++i){
      for (int j=0;j<2;++j){
        bool bad = false;
        vector<joft> p;
        for (int k=0;k<2;++k){
          djoft start = djoft(beam[i], j+2*k);
          djoft x = start;
          while (inside(x.first)){
            x = next(x);
            if (inside(x.first) && tbl[x.first.first][x.first.second]=='-')
              break;
            if (inside(x.first) && tbl[x.first.first][x.first.second]=='.')
              p.push_back(x.first);
          }
          if (inside(x.first))
            bad = true;
        }
        if (bad){
          clause(i+(1-j)*B, i+(1-j)*B);
        }
        else {
          for (int k=0;k<p.size();++k)
            choice[p[k]].push_back(i+B*j);
        }
      }
    }
    bool unsat = false;
    for (int i=0;i<r;++i)
      for (int j=0;j<c;++j)
        if (tbl[i][j]=='.'){
          vector<int> c = choice[joft(i, j)];
          if (c.size()==0)
            unsat=true;
          else if (c.size()==1)
            clause(c[0], c[0]);
          else if (c.size()==2)
            clause(c[0], c[1]);
          else{
            cout<<"What?!!!!!"<<endl;
            throw -1;
          }
        }
    for (int i=0;i<B+B;++i)
      dfs(i);
    reverse(topol.begin(), topol.end());
    for (int i=0;i<topol.size();++i){
      rdfs(topol[i], i);
    }
    for (int i=0;i<B;++i)
      if (marked[i]==marked[i+B])
        unsat = true;
    for (int i=0;i<B;++i){
      tbl[beam[i].first][beam[i].second]=(marked[i]<marked[i+B]?'|':'-');
    }
    cout<<"Case #"<<ti+1<<": "<<(unsat?"IMPOSSIBLE":"POSSIBLE")<<endl;
    if (!unsat){
      for (int i=0;i<r;++i)
        cout<<tbl[i]<<endl;
    }
  }
}
