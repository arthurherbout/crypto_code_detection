#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
struct matching{
  vector<vector<int> > f;
  vector<int> marked;
  int N;
  matching(int N=0):N(N){
    f.resize(N+N+2);
    for (int i=0;i<f.size();++i)
      f[i] = vector<int>(N+N+2, 0);
    for (int i=0;i<N;++i){
      for (int j=0;j<N;++j)
        f[i][j+N] = 0;
      f[N+N][i] = 1;
      f[i+N][N+N+1] = 1;
    }
    marked.resize(N+N+2);
  }
  void add(int a, int b){
    f[a][b+N] = 1;
  }
  void remove(int a, int b){
    for (int i=0;i<N;++i){
      f[a][i+N] = 0;
      f[i][b+N] = 0;
    }
  }
  bool match(int a, int b){
    return f[b+N][a]>0;
  }
  bool dfs(int v){
    if (marked[v])
      return false;
    marked[v] = 1;
    if (v==N+N+1)
      return true;
    for (int i=0;i<N+N+2;++i)
      if (f[v][i] && dfs(i)){
        f[v][i] -= 1;
        f[i][v] += 1;
        return true;
      }
    return false;
  }
  int optimize(){
    int cnt=0;
    while (true){
      for (int i=0;i<marked.size();++i)
        marked[i] = 0;
      if (dfs(N+N))
        cnt += 1;
      else
        break;
    }
    return cnt;
  }
};
int n;
int d1(int i, int j){
  return i+j;
}
int d2(int i, int j){
  return i+(n-1-j);
}
matching o, d;
vector<string> init, fin;
void print(vector<string> s){
  for (int i=0;i<n;++i)
    cout<<s[i]<<endl;
}
void add(vector<string>& s, int r, int c, char t){
  if (t=='.')
    return;
  if (s[r][c]=='.'){
    s[r][c]=t;
    return;
  }
  if (s[r][c]==t || s[r][c]=='o' || t=='o')
    throw -1;
  s[r][c] = 'o';
}
int main(){
  int tnum;cin>>tnum;
  for (int ti=0;ti<tnum;++ti){
    cin>>n;
    init.resize(n);
    for (int i=0;i<n;++i)
      init[i] = string(n, '.');
    o = matching(n);
    d = matching(n+n-1);
    for (int i=0;i<n;++i)
      for (int j=0;j<n;++j){
        o.add(i, j);
        d.add(d1(i, j), d2(i, j));
      }
    int m;cin>>m;
    int ans=0;
    while (m--){
      char t;int r, c;cin>>t>>r>>c;--r;--c;
      add(init, r, c, t);
      if (t=='x' || t=='o'){
        o.remove(r, c);
        ans += 1;
      }
      if (t=='+' || t=='o'){
        d.remove(d1(r, c), d2(r, c));
        ans += 1;
      }
    }
    fin = init;
    ans += o.optimize() + d.optimize();
    for (int i=0;i<n;++i)
      for (int j=0;j<n;++j){
        if (o.match(i, j))
          add(fin, i, j, 'x');
        if (d.match(d1(i, j), d2(i, j)))
          add(fin, i, j, '+');
      }
    int diff=0;
    for (int i=0;i<n;++i)
      for (int j=0;j<n;++j)
        if (init[i][j]!=fin[i][j])
          ++diff;
    cout<<"Case #"<<ti+1<<": "<<ans<<" "<<diff<<endl;
    for (int i=0;i<n;++i)
      for (int j=0;j<n;++j)
        if (init[i][j]!=fin[i][j])
          cout<<fin[i][j]<<' '<<i+1<<' '<<j+1<<endl;
  }
  return 0;
}
