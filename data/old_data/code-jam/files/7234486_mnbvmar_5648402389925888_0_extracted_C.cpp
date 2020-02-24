#include <bits/stdc++.h>

using namespace std;
#define MP make_pair
#define PB push_back
#define LL long long
#define int LL
#define st first
#define nd second
#define FI st
#define SE nd
#define FOR(i,a,b) for(int i = (a); i <= (b); i++)
#define RE(i,n) FOR(i,1,n)
#define REP(i, n) FOR(i, 0, (int)(n)-1)
#define R(i,n) REP(i,n)
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define PII pair<int,int>
#define VI vector<int>
template<class C4> void mini(C4&a4, C4 b4){a4=min(a4,b4);}
template<class C4> void maxi(C4&a4, C4 b4){a4=max(a4,b4);}

template<class TH> void _dbg(const char* sdbg, TH h){cerr<<sdbg<<"="<<h<<"\n";}
template<class TH, class... TA> void _dbg(const char* sdbg, TH h, TA... t){
  while(*sdbg!=',')cerr<<*sdbg++; cerr<<"="<<h<<","; _dbg(sdbg+1, t...);
}

template<class L, class R> ostream& operator<<(ostream& os, pair<L, R> pr) {
  return os << "(" << pr.first << ", " << pr.second << ")";
}

template<class T> ostream& operator<<(ostream& os, vector<T> V) {
  os << "["; for(auto vv : V) os << vv << ","; return os << "]";
}

using LD = long double;

#ifdef LOCAL
#define debug(...) _dbg(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) (__VA_ARGS__)
#define cerr if(0) cout
#endif

const LD kEps = 1E-9;

struct Test {
  int N;
  LD R;

  pair<LD, LD> angles(int x, int y) {
    LD fang = atan2(y, x);
    LD fdiff = abs(asin(R / hypot(x, y)));
    return make_pair(fang - fdiff, fang + fdiff);
  }

  set<pair<LD, LD>> taken;

  void run(int testid) {
    cin >> N >> R;
    R /= 1e6;

    vector<pair<int, int>> who;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i || j) { who.emplace_back(i, j); }
      }
    }

    sort(ALL(who), [&](const PII &lhs, const PII &rhs) {
            return lhs.first * lhs.first + lhs.second * lhs.second <
                   rhs.first * rhs.first + rhs.second * rhs.second;
          });

    int result = 0;

    for (auto P : who) {
      int x = P.first, y = P.second;
      LD a1, a2;
      tie(a1, a2) = angles(x, y);

      auto it = taken.lower_bound(make_pair(a1, a1)), pit = it, nit = it;
      vector<set<pair<LD, LD>>::iterator> consider;
      for (int i = 0; i < 2; i++) {
        if (pit != taken.begin()) {
          pit--;
          consider.push_back(pit);
        }
        if (nit != taken.end()) {
          consider.push_back(nit);
          nit++;
        }
      }

      bool fail = false;
      for (auto D : consider) {
        if (D->first <= a1 + kEps && a2 - kEps <= D->second) {
          fail = true;
          break;
        }
      }

      if (fail) { continue; }
      result++;

      vector<pair<LD, LD>> toRemove;

      for (auto D : consider) {
        if (D->second < a1 - kEps) { continue; }
        if (D->first > a2 + kEps) { continue; }
        toRemove.push_back(*D);
        mini(a1, D->first);
        maxi(a2, D->second);
      }

      for (auto X : toRemove) {
        taken.erase(taken.lower_bound(make_pair(X.first - kEps, 0)));
      }
      taken.emplace(a1, a2);
    }

    cout << "Case #" << testid << ": " << result << "\n";
  }
};


#undef int
int main(){
#define int LL
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout << fixed << setprecision(11);
  cerr << fixed << setprecision(6);

  int T;
  cin >> T;
  for (int i = 1; i <= T; i++) {
    Test test;
    test.run(i);
  }

  return 0;
}
