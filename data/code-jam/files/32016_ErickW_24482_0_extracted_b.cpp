#include <cmath>
#include <set>
#include <iostream>

using namespace std;

int N,M; set<int> X[2000];

char bitcount(unsigned long l)
{ register char i=0;while(l){l&=(l-1);i++;}return(i); }

int likes(int c,int i) { return (X[c].count(i) > 0); }

int happy(int c, long k) { for(int i=0;i<N;i++) { int b=(k & (1<<i))?(-i-1):(i+1); if( likes(c,b)) {return true;} } return false; }

int sat(long k) { for(int i=0;i<M;i++) { if (!happy(i,k)) return false; } return true; }

int main()
{
int C;

scanf("%d",&C);

for(int loop=1;loop<=C;loop++)
{
  scanf("%d",&N);
  scanf("%d",&M);

  for(int i=0;i<M;i++)
  {
    int T;
    X[i].clear();
    scanf("%d",&T);
    for(int j=0;j<T;j++)
    {
      int x,y;
      scanf("%d%d",&x,&y);
      X[i].insert(y?(-x):x);
    }
  }

  int m = 99999999; int mi=-1;
  for(int i=0;i<(1<<N);i++)
  {
    if (sat(i)) { int l=bitcount(i); if(l<m) {m=l; mi=i;} }
  }

  if (mi < 0) { printf("Case #%d: IMPOSSIBLE\n",loop); }
  else { printf("Case #%d:",loop); for(int i=0;i<N;i++) printf(" %d",!!(mi&(1<<i))); putchar(10); }
}

}
