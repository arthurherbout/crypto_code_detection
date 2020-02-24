#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std; typedef unsigned long ul; typedef long long ll;
typedef unsigned long long ull;

double baz(double r1, double r2)
{
  double t = acos ( (r1*r1 + 1 - r2*r2) / (2*r1) );
  double o =  r1 * r1 * t - 0.5 * r1 * r1 * sin(2*t);

  return o;
}

double bar(double r1, double r2)
{
  if (r1 > r2) return bar(r2,r1);
  if (r2 >= r1+1) return M_PI*r1*r1;
  return baz(r1,r2) + baz(r2,r1);
  return 0;
}

double foo(double d, double r1, double r2) { return d*d*bar(r1/d, r2/d); }

int main()
{
int cases;

cin >> cases; getchar();

for(int loop=1;loop<=cases;loop++)
{
  int n, m;

  cin >> n >> m;
  double px[n], py[n]; double qx, qy;
  for(int i=0;i<n;i++) cin >> px[i] >> py[i];

  double d = hypot(px[0]-px[1], py[0]-py[1]);

  printf("Case #%d: ",loop);

  for(int j=0;j<m;j++) 
  {
    cin >> qx >> qy;
    printf("%0.8f ", foo(d,
     hypot(qx - px[0], qy - py[0]),
     hypot(qx - px[1], qy - py[1]) ));
  }

  puts("");

  fflush(stdout);
}

}
