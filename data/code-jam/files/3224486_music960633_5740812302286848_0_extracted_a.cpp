#include <stdio.h>
#include <math.h>

double x[1001], y[1001], z[1001];
double vx[1001], vy[1001], vz[1001];

double sqr(double i){ return i*i; }
double distance(int i, int j){
   return sqrt(sqr(x[i]-x[j]) + sqr(y[i]-y[j]) + sqr(z[i]-z[j]));
}

int par[1001];

void init(int n){
   for(int i=0; i<n; ++i)
      par[i] = i;
}
int root(int i){
   if(par[i] == i) return i;
   return par[i] = root(par[i]);
}
void merge(int i, int j){
   int ri = root(i);
   int rj = root(j);
   if(ri != rj) par[ri] = rj;
}

bool check(int n, double dis){
   init(n);
   for(int i=0; i<n; ++i)
      for(int j=i; j<n; ++j)
         if(distance(i, j) <= dis)
            merge(i, j);
   return root(0) == root(1);
}

int main(){
   int T, N, S;
   double lft, rgt, mid;
   scanf("%d", &T);
   for(int t=1; t<=T; ++t){
      scanf("%d%d", &N, &S);
      for(int i=0; i<N; ++i)
         scanf("%lf%lf%lf%lf%lf%lf", &x[i], &y[i], &z[i], &vx[i], &vy[i], &vz[i]);
      lft = 0.0;
      rgt = 10000.0;
      for(int i=0; i<100; ++i){
         double mid = (lft + rgt) / 2;
         if(check(N, mid)) rgt = mid;
         else lft = mid;
      }
      printf("Case #%d: %f\n", t, lft);
   }
   return 0;
}
