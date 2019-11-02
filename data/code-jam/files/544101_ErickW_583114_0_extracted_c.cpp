#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std; typedef unsigned long ul; typedef long long ll;
typedef unsigned long long ull;

int doit(int i, int j)
{
  if (i==j) return 0; else if (i>j) return doit(j,i);
  if (j/i > 1) return 1; else return !doit(j%i,i);
}

int main()
{
int cases;

cin >> cases; getchar();

for(int loop=1;loop<=cases;loop++)
{
  int a1,a2,b1,b2;
  cin >> a1 >> a2>>b1>>b2;
  long long t=0;

  for(int i=a1;i<=a2;i++) for(int j=b1;j<=b2;j++) if (doit(i,j)) t++;

  printf("Case #%d: %lld",loop,t);

  puts("");
  fflush(stdout);
}

}
