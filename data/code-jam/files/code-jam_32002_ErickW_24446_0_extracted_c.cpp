#include <cmath>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

#define two(x) (1<<(x))

using namespace std; typedef unsigned long ulong; typedef long long llong;

char mp[20][20];
int H[10][1<<11];
int block[10];
int m,n;

char bitcount(int l)
{register char i=0;while(l){l&=(l-1);i++;}return(i);}

int main()
{
int cases;

cin >> cases; getchar();

for(int loop=1;loop<=cases;loop++)
{
  int i,j,k;

  cin >> m >> n;
  for(i=0;i<m;i++) { scanf("%s",mp[i]); }

  memset(block,0,sizeof(block));
  memset(H,0,sizeof(H));

  for(i=0;i<m;i++) for(j=0;j<n;j++) if (mp[i][j]=='x') block[j] |= two(i);

  for(i=0;i<two(m);i++)
  {
    if (i & block[0]) H[0][i]=-10000; else H[0][i] = bitcount(i);
  }

  for(j=1;j<n;j++)
  {
    for(k=0;k<two(m);k++)
    {
      if (k & block[j]) { H[j][k]=-10000; continue; }

      int c=0;

      for(i=0;i<two(m);i++)
      {
        if ( (i&k) || ((i<<1)&k) || (i&(k<<1))) continue; else { c=max(c,H[j-1][i]); }
      }
      H[j][k] = c+bitcount(k);
    }
  }

//  for(i=0;i<two(m);i++) { for(j=0;j<n;j++) printf(" %d",H[j][i]);  printf("\n"); }

  int c=0;
  for(i=0;i<two(m);i++)
  {
    c=max(c,H[n-1][i]);
  }

  printf("Case #%d: %d\n",loop,c);

  fflush(stdout);
}

}
