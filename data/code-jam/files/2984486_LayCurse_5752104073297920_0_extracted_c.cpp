#include<cstdio>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<utility>
#include<iostream>
#include<cmath>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)


void test1(int n, int t){
  int i, j, k; double T = t;
  vector<int> p;
  map< vector<int>,int > freq;
  int per[100][100] = {};

  rep(i,n) p.push_back(i);

  do{
    freq[p] = 0;
  }while(next_permutation(p.begin(),p.end()));

  while(t--){
    rep(i,n) p[i] = i;
    rep(i,n){
      k = rand() % n;
      swap(p[i], p[k]);
    }
    freq[p]++;
    rep(i,n) per[i][p[i]]++;
  }

  rep(i,n) T /= (i+1);

  rep(i,n) p[i] = i;
  do{
    rep(i,n) printf("%d ",p[i]);
    printf(": %f\n", freq[p]/T);
  }while(next_permutation(p.begin(),p.end()));

  rep(i,n){ rep(j,n) printf("%f ",per[i][j]/T); puts(""); }
}

void test2(int n, int t){
  int i, j, k, loop; double T = t;
  vector<int> p;
  static int good[2000000], bad[2000000];

  rep(i,n) p.push_back(i);

  rep(i,2000000) good[i] = bad[i] = 0;

  rep(loop,t){
    rep(i,n) p[i] = i;
    rep(i,n){
      k = rand() % n; // bad
      swap(p[i], p[k]);
    }
    j = 0;
    rep(i,n) j += abs(p[i]-i);
    bad[j]++;
  }

  rep(loop,t){
    rep(i,n) p[i] = i;
    rep(i,n){
      k = i + rand() % (n-i); // good
      swap(p[i], p[k]);
    }
    j = 0;
    rep(i,n) j += abs(p[i]-i);
    good[j]++;
  }

  rep(i,n*n){
    if(good[i]/T + bad[i]/T > 0.0001)
      printf("%3d : good %f, bad %f\n", i, good[i]/T, bad[i]/T);
  }

  double up, down, all;
  up = down = all = 0;
  rep(i,327000) down += good[i], all += good[i];
  REP(i,327000,1000000) up += good[i], all += good[i];
  printf("good %f %f\n",down/all,up/all);
  
  up = down = all = 0;
  rep(i,327000) down += bad[i], all += bad[i];
  REP(i,327000,1000000) up += bad[i], all += bad[i];
  printf("bad %f %f\n",down/all,up/all);
}


double Gper[1200][1200];

void test3(int n, int t){
  int i, j, k, loop; double T = t;
  vector<int> p;
  static int good[2000000], bad[2000000], vis[2000];
  static int per[1200][1200] = {};
  int lon;

  rep(i,n) p.push_back(i);

  rep(i,2000000) good[i] = bad[i] = 0;

  rep(loop,t){
    rep(i,n) p[i] = i;
    rep(i,n){
      k = rand() % n; // bad
      swap(p[i], p[k]);
    }

    rep(i,n) per[i][p[i]]++;
  }

  rep(i,n) rep(j,n) Gper[i][j] = (double)per[i][j] / t * n;
//  rep(i,n){ rep(j,n) printf("%f ",(double)per[i][j]/t*n); puts(""); }

}

int main(){
  int i,j,k;
  int T, cnt = 0;
  int N, p[2000], sum;
  double res;

//  test1(8, 1000000);
//  return 0;
  
//  test2(1000, 1000000);
//  return 0;
  
  test3(1000, 1000000);
  
  scanf("%d",&T);
  while(T--){
    fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d",&N);
    rep(i,N) scanf("%d",p+i);
    res = 1;
    rep(i,N) res *= Gper[i][p[i]];
    if(res <= 1) puts("GOOD"); else puts("BAD");
  }

  return 0;
}
