#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std; typedef unsigned long ul; typedef long long ll;
typedef unsigned long long ull;

int L[1000001], R[1000001];

int doit(int i, int j)
{
  if (i==j) return 0; else if (i>j) return doit(j,i);
  if (j/i > 1) return 1; else return !doit(j%i,i);
}

int main()
{
int cases;

cin >> cases; getchar();

  for(int i=1;i<=1000000;i++)
  {
    int l0=1, l1=i, r0=i, r1=2*i, l, r;
    while(l1-l0>1) { l=(l1+l0)/2; if(doit(i,l)) l0=l; else l1=l; }
    while(r1-r0>1) { r=(r1+r0)/2; if(!doit(i,r)) r0=r; else r1=r; }
    l = l1; r = r1-1;
    L[i]=l; R[i]=r;
  }

for(int loop=1;loop<=cases;loop++)
{
  int a1,a2,b1,b2;
  cin >> a1 >> a2>>b1>>b2;
  long long t=0;


  for(int i=a1;i<=a2;i++)
  {
    int l = L[i], r=R[i];
    t += b2 - b1 + 1;
    t -= (( b2 <? r ) - ( b1 >? l) +1) >? 0;
  }
  printf("Case #%d: %lld",loop,t);

  puts("");
  fflush(stdout);
}

}
