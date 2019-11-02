#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#define two(x) (1<<(x))

using namespace std; typedef unsigned long ulong; typedef long long llong;
typedef unsigned long long ullong;

double p[30][4];
double x[4096];
int picks[30];

int main()
{
int cases;

cin >> cases;

for(int loop=1;loop<=cases;loop++)
{
  int m,q;

  cin >> m >> q;
  //printf("%d %d\n",m,q);

  for(int i=0;i<q;i++) for(int j=0;j<4;j++) 
    { cin >> p[i][j]; }

  memset(x,0,sizeof(x));
  for(int i=0;i<(1<<(2*q));i++)
  {
    for(int j=0;j<q;j++) picks[j] = (i>>(2*j))&3;
    x[i]=1.0; for(int j=0;j<q;j++) x[i]*=p[j][picks[j]];
  }

  sort(x,x+two(2*q));
  reverse(x,x+two(2*q));

  double P=0.0;
  for(int i=0;i<m;i++) P+=x[i];

  printf("Case #%d: ",loop);
  printf("%0.6lf",P);

  puts("");
  fflush(stdout);
}

}
