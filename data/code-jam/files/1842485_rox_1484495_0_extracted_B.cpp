#include <cstdio>
#include <map>
#include <algorithm>
#include <complex>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;
typedef long long ll;
typedef complex<double> P;

pair<int,int> r[1111];

struct C {
  P p;
  int i;
  C(const P &p,int i):p(p),i(i){}
};

double get(int w)
{
  return ((double)rand()/RAND_MAX)*w;
}

int main(void)
{
  srand(time(NULL));
  int t;
  scanf("%d",&t);
  for(int T=1; T<=t; T++){
    printf("Case #%d:",T);
    int n,w,l;
    
    scanf("%d%d%d",&n,&w,&l);
    for(int i = 0; i < n; i++){
      int v;
      scanf("%d",&v);
      r[i] = make_pair(v,i);
    }

    vector<C> c;

    for(int i = 0; i < n; i++){
      c.push_back(C(P(rand()%w+1,rand()%l+1),i));
    }

    for(;;) {
      bool dame = false;
      for(int i = 0; i < n && !dame; i++){
        for(int j = i+1; j < n && !dame; j++){
          if( abs(c[i].p-c[j].p) < r[i].first+r[j].first ) {
            dame = true;
          }
        }
      }

      if( !dame )break;

      C &p = c[rand()%n];

      p.p.real() = get(w);
      p.p.imag() = get(l);

      //printf("%f %f\n",p.p.real(),p.p.imag());
      
    }

    for(int i = 0; i < n; i++){
      printf(" %f %f",c[i].p.real(),c[i].p.imag());
    }
    puts("");

    fflush(stdout);
    
  }
  return 0;
}
