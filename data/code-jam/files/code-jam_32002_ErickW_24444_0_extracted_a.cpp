#include <cmath>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <complex>

#define M 300

using namespace std; typedef unsigned long ulong; typedef long long llong;

complex<int> L(0,1),R(0,-1);

char canvas[M][M];
int ns[M][M];
int ew[M][M];

int main()
{
int cases;

cin >> cases; getchar();

for(int loop=1;loop<=cases;loop++)
{
  int i,j,k;
  complex<int> v(0,-1);
  int x=M/2, y=M/2;

  int l; cin >> l;
  string s[l];
  int t[l];

  memset(canvas,0,sizeof(canvas));
  memset(ns,0,sizeof(ns));
  memset(ew,0,sizeof(ew));

  for(i=0;i<l;i++) cin >> s[i] >> t[i];

  for(i=0;i<l;i++) {
    for(j=0;j<t[i];j++) {
      for(k=0;k<s[i].length();k++)
      {
        char c=s[i][k];

        if (c!='F') { if (c=='L') v*=L; else v*=R; }
        else
        {
          int xp = x+v.real(), yp = y+v.imag();
          if (xp==x) { ns[x][min(yp,y)] = 1; }
          else { ew[min(xp,x)][y] = 1; }
          x=xp,y=yp;
        }
      }
    }
  }

  for(i=0;i<M;i++)
  {
    int mj=M, Mj=-1;
    int in=0;
    for(j=0;j<M;j++) if (ew[i][j]) { mj=min(mj,j); Mj=max(Mj,j); }

    for(j=mj;j<Mj;j++)
    {
      if (ew[i][j]) in = !in;
      if (in) canvas[i][j] = 1; else canvas[i][j] = 2;
    }
  }

  for(i=0;i<M;i++)
  {
    int mj=M, Mj=-1;
    int in=0;
    for(j=0;j<M;j++) if (ns[j][i]) { mj=min(mj,j); Mj=max(Mj,j); }

    for(j=mj;j<Mj;j++)
    {
      if (ns[j][i]) in = !in;
      if (in) canvas[j][i] = 1; else canvas[j][i] = 2;
    }
  }

 //for(i=0;i<M;i++) { for(j=0;j<M;j++) printf("%d",canvas[i][j]); putchar('\n'); }
 int area = 0;
 for(i=0;i<M;i++) { for(j=0;j<M;j++) if (canvas[i][j]==2) area++; }

  printf("Case #%d: %d\n",loop,area);

  fflush(stdout);
}

}
