#include <bits/stdc++.h>

using namespace std;
#define PB push_back
#define MP make_pair
#define LL long long
#define int LL
#define FOR(i,a,b) for (int i = (a); i <= (b); i++)
#define REP(i,n) FOR(i,0,(int)(n)-1)
#define RE(i,n) FOR(i,1,n)
#define R(i,n) REP(i,n)
#define FI first
#define SE second
#define st FI
#define nd SE
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define VI vector<int>
#define PII pair<int,int>
#define LD long double

template<class C> void mini(C& a4, C b4) { a4 = min(a4, b4); }
template<class C> void maxi(C& a4, C b4) { a4 = max(a4, b4); }

template<class TH> void _dbg(const char *sdbg, TH h){cerr<<sdbg<<"="<<h<<"\n";}
template<class TH, class... TA> void _dbg(const char *sdbg, TH h, TA... a) {
  while(*sdbg!=',')cerr<<*sdbg++;cerr<<"="<<h<<","; _dbg(sdbg+1, a...);
}

template<class T> ostream &operator<<(ostream &os, vector<T> V){
  os<<"[";for(auto vv:V)os<<vv<<",";return os<<"]";
} 

template<class L, class R> ostream &operator<<(ostream &os, pair<L,R> P) {
  return os << "(" << P.st << "," << P.nd << ")";
}


#ifdef LOCAL
#define debug(...) _dbg(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) (__VA_ARGS__)
#define cerr if(0)cout
#endif

const int MOD = 1e9 + 7;
int powMod(int a, int n, int Mod) {
  int res = 1; while (n) {
    if (n & 1) { res = ((LL)res * a) % Mod; }
    a = ((LL)a * a) % Mod; n >>= 1;
  } return res;
}
int invMod(int a, int Mod) { return powMod(a, Mod - 2, Mod); }
PII gaussMod(vector<VI> matrix, int Mod) {
  int N = SZ(matrix) - 1, M = SZ(matrix[0]) - 1;
  vector<bool> visited(N);
  VI wrow(M);
  int rank = M, det = 1, ssign = 0;
  REP (col, M) {
    int which = -1;
    REP (row, N) {
      if (visited[row] || !matrix[row][col]) { continue; }
      which = row; break;
    }
    if (which == -1) { rank--; continue; }
    wrow[col] = which;
    visited[which] = true;
    det = ((LL)det * matrix[which][col]) % Mod;
    REP (row, N) {
      if (row == which || !matrix[row][col]) { continue; }
      int coef = Mod - ((LL)matrix[row][col] * invMod(matrix[which][col], Mod)) % Mod;
      REP (c, M) {
        matrix[row][c] = (matrix[row][c] + (LL)matrix[which][c] * coef) % Mod;
      }
    }
  }
  if (rank != M || N != M) { return {0, rank}; }
  REP (i, N) { FOR (j, i, N - 1) { if (wrow[i] > wrow[j]) { ssign++; } } }
  if (ssign & 1) { det = Mod - det; }
  return {det, N};
}


vector<vector<int>> res[10000];

struct Sol{
  void run(int cas){
    cout << "Case #" << cas << ": ";
    int n;
    cin >> n;
    if(n == 58){
      cout << "8\n";
      cout << "01100000\n";
      cout << "10100100\n";
      cout << "11010000\n";
      cout << "00101001\n";
      cout << "00010100\n";
      cout << "01001010\n";
      cout << "00000101\n";
      cout << "00010010\n";
      return;
    }
    cout << SZ(res[n]) << endl;
    R(i,SZ(res[n])){
      R(j,SZ(res[n])){
        if(i == j)
          cout << 0;
        else
          cout << -res[n][i][j];
      }
      cout << "\n";
    }
  }
};

void add_edge(vector<vector<int>> &x,int a,int b){
  x[a][a]++;
  x[b][b]++;
  x[a][b]--;
  x[b][a]--;
}


int mam = 2;
bool dodaj(vector<vector<int>> &pom){
  debug("dodaj",pom);
  int il = gaussMod(pom,MOD).FI;
  if(il < 0)il += MOD;
  if(il > 10000)return 1;
  if(il <= 2)return 0;
  if(res[il].empty()){
    int il2 = gaussMod(pom,MOD+2).FI;
    if(il2 < 0)il2 += MOD+2;
    if(il != il2)return 1;
    mam++;
    //cout << mam << endl;
    res[il] = pom;
  }
  return 0;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout << fixed << setprecision(11);
  cerr << fixed << setprecision(6);
//   R(i,10)R(j,10)R(k,10)R(l,10){
//     if(i * j * l + j * k * l + i * k * l + i * j * k == 58){
//       cout << i << j << k << " "<< l << "\n";
//     }
//   }
//   return 0;
//   vector<vector<int>> zly(13,vector<int>(13));
//   R(i,10){
//     add_edge(zly,i,(i+1)% 10);
//   }
//   add_edge(zly,4,10);
//   add_edge(zly,11,10);
//   add_edge(zly,11,12);
//   add_edge(zly,12,0);
//   int il = gaussMod(zly,MOD).FI + MOD;
//   cout << il << "\n";;
//   return 0;
//   R(_,1000000){
//     int n = rand() % 6 + 1;
//     vector<vector<int>> pom(n,vector<int>(n));
//     R(i,n-1) add_edge(pom, i+1, rand() % (i+1));
//     while(1){
//       if(dodaj(pom))break;
//       R(i,5){
//         int a = rand()%n;
//         int b = rand()%n;
//         if(a != b && pom[a][b] == 0){
//           add_edge(pom,a,b);
//           goto end2;
//         }
//       }
//       break;
//       end2:;
//     }
//     debug(pom);
//     dodaj(pom);
//   }
//   R(i,200){
//     if(res[i].empty()){
//       cout << i << endl;
//     }
//   }
//   return 0;
  while(mam != 9999){
    int n = rand()%20 + 3;
    assert(n <= 22);
    vector<vector<int>> pom(n,vector<int>(n));
    {
      int a = rand() % n;
      if(a <= 2 || a >= n-2){
        R(i,n)add_edge(pom,i,(i+1)%n);
      }else{
        R(i,n-1)add_edge(pom,i,i+1);
        add_edge(pom,0,a);
        add_edge(pom,a,n-1);
      }
    }
    while(1){
      if(dodaj(pom))break;
      R(i,5){
        int a = rand()%n;
        int b = rand()%n;
        if(a != b && pom[a][b] == 0){
          add_edge(pom,a,b);
          goto end;
        }
      }
      break;
      end:;
    }
  }
 // return 0;
  int t;
  cin >> t;
  R(i,t){
    Sol sol;
    sol.run(i + 1);
  }
}
