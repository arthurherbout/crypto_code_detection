#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

inline long long myabs(long long x) {
   if (x >= 0)
      return x;
   return -x;
}

double** dist;

struct Sol
{
   void read()
   {
      cin >> N;
      for (long long i = 0; i < N; ++i)
         cin >> x[i] >> y[i];
/*         dist = new double*[N];
         for (long long i = 0; i < N; ++i) {
            dist[i] = new double[N];
         }*/
         for (long long i = 0; i < N; ++i) {
            for (long long j = 0; j < i; ++j) {
               dist[i][j] = dist[j][i] = sqrt(((double)x[i]-x[j])*((double)x[i]-x[j])+((double)y[i]-y[j])*((double)y[i]-y[j]));
            }
         }
   }

   inline bool checkBoundBox(double bd, long long c, long long p) {
      if (myabs(x[p] - x[c]) > bd || myabs(y[p] - y[c]) > bd)
         return false;
      return true;
   }

   bool boundSolveSub(double bd, long long c, double& ans) 
   {
      double mm = 1e+12;
      int cp[10000];
      int cpcnt = 0;
      for (int i = 0; i < N; ++i)
         if (checkBoundBox(bd, c, i))
            cp[cpcnt++] = i;
      for (int i = 0; i < cpcnt; ++i) {
         int ni = cp[i];
         for (int j = 0; j < i; ++j) {
            int nj = cp[j];
            if (dist[ni][nj] >= mm) continue;
//            if (dist(ni, nj) >= mm) continue;
            for (int k = 0; k < j; ++k) {
               int nk = cp[k];
               double a = dist[ni][nj] + dist[ni][nk] + dist[nj][nk];
//               double a = dist(ni, nj) + dist(ni, nk) + dist(nj, nk);
               if (a > 2 * bd) continue;
               if (a < mm)
                  mm = a;
            }
         }
      }
      if (mm > 1e+11) return false;
      ans = mm;
      return true;
   }

   bool boundSolve(double bd, double& ans)
   {
      ans = 1e+12;
      bool okay = false;
      for (long long i = 0; i < N; ++i) {
         double ta;
         if (boundSolveSub(bd, i, ta)) {
            okay = true;
            if (ans > ta) ans = ta;
         }
      }
      return okay;
   }

   void solve(long long cn)
   {
      double ans = 0;
      double bd = 0;
      if (N >= 100) {
         /*for (int i = 0; i < N; ++i) 
            for (int j = 0; j < i; ++j) {
               if (bd < myabs(x[i] - x[j]))
                  bd = myabs(x[i] - x[j]);
               if (bd < myabs(y[i] - y[j]))
                  bd = myabs(y[i] - y[j]);
            }*/
         for (int i = 0; i < N; ++i) {
            if (bd < myabs(x[i]))
               bd = myabs(x[i]);
            if (bd < myabs(y[i]))
               bd = myabs(y[i]);
         }
         bd /= 10000;
         bd += 1;
      }
      else {
         bd = 1e+13;
      }
      cerr << bd << endl;
      while (!boundSolve(bd, ans)) {
         bd *= 5;
         cerr << bd << endl;
      }
      printf("Case #%lld: %.6lf\n", cn, ans);
      fprintf(stderr, "Case #%lld: %.6lf\n", cn, ans);
      /*for (int i = 0; i < N; ++i)
         delete [] dist[i];
      delete [] dist;*/
   }

   long long N;
   long long x[10000], y[10000];
 //  double** dist;
/*
   inline double dist(long long i, long long j) {
      return sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
   }
 */ 
};

int main()
{
   long long T;
   cin >> T;
   dist = new double*[10000];
   for (int i = 0; i < 10000; ++i)
      dist[i] = new double[10000];
   for (long long t = 1; t <= T; ++t) {
      Sol s;
      s.read();
      s.solve(t);
   }
   return 0;
}
