#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

/* GCJ template part 02 -- Common macro definitions */
#define FOR(i, a, b)    for(int i = int(a); i < int(b); ++i)
#define REP(i, n)       FOR(i, 0, n)
#define FORE(i, a, b)   for(int i = int(a); i <= int(b); ++i)
#define REPE(i, n)      FORE(i, 0, n)
#define FORD(i, a, b)   for(int i = int(b) - 1; i >= int(a); --i)
#define REPD(i, n)      FORD(i, 0, n)
#define FORDE(i, a, b)  for(int i = int(b); i >= int(a); --i)
#define REPDE(i, n)     FORDE(i, 0, n)
#define IT(c)           __typeof((c).begin())
#define FORIT(i, c)     for(IT(c) i = (c).begin(); i != (c).end(); ++i)
#define SZ(c)           (int((c).size()))
#define ALL(c)          (c).begin(), (c).end()
#define SET(m, v)       memset((m), (v), sizeof(m))
#define REVERSE(c)      reverse(ALL(c))
#define SORT(c)         sort(ALL(c))
#define UNIQ(c)         SORT(c), (c).erase(unique(ALL(c)), (c).end())
#define PB              push_back
#define MP              make_pair
#define BIT(x)          (1<<(x))
#define MAPRET(m, x)    { IT(m) _ = m.find(x); if(_ != m.end()) return _->second; }

/* GCJ template part 03 -- Common type definitions */
typedef long long LL;
typedef unsigned long long ULL;
typedef vector<int> VI;
typedef vector<LL> VLL;
typedef vector<string> VS;
typedef vector<double> VD;
typedef vector<VI> VVI;
typedef vector<VLL> VVLL;
typedef vector<VS> VVS;
typedef vector<VD> VVD;

// Tests if string a starts with string b
inline bool startswith(const string &a, const string &b) {
    return a.size() >= b.size() && a.compare(0, b.size(), b) == 0;
}

// Tests if string a end with string b
inline bool endswith(const string &a, const string &b) {
    return a.size() >= b.size() && a.compare(a.size() - b.size(), b.size(), b) == 0;
}

int tab[37][37];
int plan[37];
int plan2[37];

int bestc, bestt;
int P;

void recu(int pl, int n)
{
     //arrivé à planete pl
     //on threaten toutes les planetes reliés
     if (n > bestc) return;
     if (plan2[pl] == 1) return;
     if (plan[pl] > P) return;
     
     int end = 0;
     plan2[pl] = 1;
     FORE(i, 0, P)
     { if ((i != pl) && (tab[pl][i] == 1)) {plan[i]++; if (i==1) end = 1;} }

     if (end == 1) {
          int num = 0;
          FORE(i, 0, P)
          {if (plan[i] > 0) num++;}
          
          if ((n <= bestc) && (num-n > bestt)) {
                 bestc = n;
                 bestt = num-n;
          }
        
     } else {
     //on recu
     FORE(i, 0, P)
     { if ((i != pl) && (tab[pl][i] == 1)) {recu(i,n+1);} }
     }
          plan2[pl] = 0;
     FORE(i, 0, P)
     { if ((i != pl) && (tab[pl][i] == 1)) {plan[i]--;} }

}

int main()
{
    int T;

    cin >> T;

    FORE(i, 1, T)
    {
        int W;
        
        cin >> P >> W;
        FORE(j, 0, 36) {plan[j] = 0; plan2[j] = 0;FORE(k, 0, 36) {tab[j][k] = 0;}}
        
        FORE(j, 1, W)
        {
          int A, B;
          scanf("%d,%d", &A, &B);
          tab[A][B] = 1;        
          tab[B][A] = 1;   
        }
        bestc = 37;
        bestt = 0;
        
        plan[0] = P;
        recu(0, 0);
        
        cout<<"Case #"<<i<<": "<<bestc<<" "<<bestt-1<<endl;
    }

    return 0;
}
