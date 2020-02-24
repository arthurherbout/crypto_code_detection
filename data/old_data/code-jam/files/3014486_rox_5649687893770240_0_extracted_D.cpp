#include <cstdio>
#include <set>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

int n,m;
char s[1111][111];
int la[1111];
int res2 = 0;
//set<vector<set<string>>>VV;

vector<set<string>> calc()
{
  vector<string> p[4];
  vector<set<string>> t(n);
  for( int i = 0; i < m; i++ ) {
    p[la[i]].push_back(s[i]);
  }
  for( int i = 0; i < n; i++ ) {
    for( int k = 0; k < int(p[i].size()); k++ ) {
      for( int j = 0; j <= int(p[i][k].size()); j++ ) {
        t[i].insert(p[i][k].substr(0,j));
      }
    }
  }
  return t;
}

int res;

void labe(int a,int u)
{
  if( a == m ) {
    if( u != (1<<n)-1 ) return;
    auto a = calc();
    int aaa = 0;
    for( int i = 0; i < n; i++ ) aaa += a[i].size();
    for( int i = 0; i < n; i++ ) {
      //for( auto p : a[i] ) cout << "," << p;
      //cout << endl;
    }
    //VV.insert(a);
    if( aaa == res ) ++res2;
    if( res < aaa ) {res = aaa; res2=1;}
    return;
  }
  for( int i = 0; i < n; i++ ) {
    la[a] = i;
    labe(a+1,u|1<<i);
  }
}

int main(void)
{
  int T;
  scanf("%d",&T);
  for( int T_ = 1; T_ <= T; T_++ ) {
    printf("Case #%d: ",T_);
    scanf("%d%d",&m,&n);
    for( int i = 0; i < m; i++ ) {
      scanf("%s",s[i]);
    }
    res = 0;
    labe(0,0);
    printf("%d %d\n",res,res2);
  }
  return 0;
}
