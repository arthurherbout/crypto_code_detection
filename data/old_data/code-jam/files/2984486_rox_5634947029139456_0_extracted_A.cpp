/*
3
3 2
01
11
10

10
01
11

2 3
101
111

010
001
*/
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int n,l;

bool C(int a[],int b[],int u)
{
  vector<int> p;
  for( int i = 0; i < n; i++ ) p.push_back(a[i]^u);
  sort(p.begin(),p.end());
  sort(b,b+n);
  for( int i = 0; i < n; i++ ) if( p[i] != b[i] ) return false;
  return true;
}

int main(void)
{
  int T,res;
  int v[11],w[11];
  char vv[11];
  scanf("%d",&T);
  for( int tt = 1; tt <= T; tt++ ) {
    printf("Case #%d: ",tt);
    scanf("%d%d",&n,&l);
    res = -1;
    for( int i = 0; i < n; i++ ) {
      scanf("%s",vv);
      v[i] = 0;
      for( int j = 0; j < l; j++ ) {
        v[i] |= (vv[j]-'0')<<j;
      }
    }
    for( int i = 0; i < n; i++ ) {
      scanf("%s",vv);
      w[i] = 0;
      for( int j = 0; j < l; j++ ) {
        w[i] |= (vv[j]-'0')<<j;
      }
    }
    for( int i = 0; i < (1<<l); i++ ) {
      //printf("%d\n",i);
      if( C(v,w,i) ) {
        if( res == -1 || (res>__builtin_popcount(i))) res = __builtin_popcount(i);
      }
    }
    if( res < 0 ) {
      puts("NOT POSSIBLE");
    } else {
      printf("%d\n",res);
    }
  }
  return 0;
}
