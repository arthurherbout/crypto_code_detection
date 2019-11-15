#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int n,l;

bool C(long long a[],long long b[],long long u)
{
  vector<long long> p;
  for( int i = 0; i < n; i++ ) p.push_back(a[i]^u);
  sort(p.begin(),p.end());
  for( int i = 0; i < n; i++ ) if( p[i] != b[i] ) return false;
  return true;
}

int main(void)
{
  int T,res;
  long long v[222],w[222];
  char vv[222];
  scanf("%d",&T);
  for( int tt = 1; tt <= T; tt++ ) {
    printf("Case #%d: ",tt);
    scanf("%d%d",&n,&l);
    res = -1;
    for( int i = 0; i < n; i++ ) {
      scanf("%s",vv);
      v[i] = 0;
      for( int j = 0; j < l; j++ ) {
        v[i] |= (long long)(vv[j]-'0')<<j;
      }
    }
    for( int i = 0; i < n; i++ ) {
      scanf("%s",vv);
      w[i] = 0;
      for( int j = 0; j < l; j++ ) {
        w[i] |= (long long)(vv[j]-'0')<<j;
      }
    }
    sort(w,w+n);
    if( C(v,w,0) ) {
      res = 0;
    }
    for( int i = 0; i < n; i++ ) {
      for( int j = 0; j < n; j++ ) {
        //printf("%d\n",i);
        long long u = v[i]^w[j];
        if( C(v,w,u) ) {
          if( res == -1 || (res>__builtin_popcountll(u))) res = __builtin_popcountll(u);
        }
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
