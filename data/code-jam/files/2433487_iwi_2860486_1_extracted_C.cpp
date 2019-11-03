#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cassert>
#include <cstdarg>
#include <climits>
#include <sys/time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

#define BEGIN_SOLVER struct solver {
#define END_SOLVER };

const int MAX_OUT = 10000;
const char *OUTPUT_FORMAT = "Case #%d: %s"; //"Case #%d:\n%s";

#define all(c) (c).begin(), (c).end()
#define iter(c) __typeof((c).begin())
#define present(c, e) ((c).find((e)) != (c).end())
#define cpresent(c, e) (find(all(c), (e)) != (c).end())
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define tr(c, i) for (iter(c) i = (c).begin(); i != (c).end(); ++i)
#define pb push_back
#define mp make_pair

typedef long long ll;



void init() {}



BEGIN_SOLVER ///////////////////////////////////////////////////////////////////////////////////////

int N, M, P;
int U[2010], V[2010], A[2010], B[2010];
int C[510];

void input() {
  scanf("%d%d%d", &N, &M, &P);
  rep (i, M) {
    cin >> U[i] >> V[i] >> A[i] >> B[i];
    --U[i];
    --V[i];
  }
  rep (i, P) {
    cin >> C[i];
    --C[i];
  }
}

bool inp[2010];
vector<pair<int, int> > adj[2010];
int pot1[2010], pot2[2010];


bool check(int md) {
  memset(inp, 0, sizeof(inp));
  rep (i, md) inp[C[i]] = true;

  rep (v, N) adj[v].clear();
  rep (i, M) adj[U[i]].pb(mp(V[i], inp[i] ? A[i] : B[i]));

  multimap<int, int> que;
  rep (v, N) pot1[v] = INT_MAX;
  que.insert(mp(0, 0));
  pot1[0] = 0;
  while (!que.empty()) {
    int d = que.begin()->first;
    int v = que.begin()->second;
    que.erase(que.begin());
    if (d > pot1[v]) continue;

    rep (i, adj[v].size()) {
      int tw = adj[v][i].first;
      int td = d + adj[v][i].second;
      if (td < pot1[tw]) {
        que.insert(mp(td, tw));
        pot1[tw] = td;
      }
    }
  }

  vector<pair<int, int> > ord(N);
  rep (v, N) ord[v] = mp(pot1[v], v);
  sort(all(ord));
  reverse(all(ord));

  vector<bool> in_spt(N);
  in_spt[1] = true;
  rep (k, N) {
    int v = ord[k].second;
    rep (i, adj[v].size()) {
      int c = adj[v][i].second;
      int w = adj[v][i].first;
      if (in_spt[w] && pot1[w] - pot1[v] == c) {
        assert(0 <= v && v < N);
        in_spt[v] = true;
      }
    }
  }

  // debug("md = %d\n", md); rep (v, N) debug("%d: %d\n", v + 1, pot1[v]);

  // Check the blue edges
  rep (i, md) {
    int k = C[i];
    if (pot1[V[k]] - pot1[U[k]] < A[k]) return false;
  }

  //
  // 2nd Dijkstra!!
  //
  rep (v, N) adj[v].clear();
  rep (i, M) adj[U[i]].pb(mp(V[i], A[i]));

  que.clear();
  rep (v, N) pot2[v] = INT_MAX;
  int s = V[C[md - 1]];
  que.insert(mp(pot1[s], s));
  pot2[s] = pot1[s];
  while (!que.empty()) {
    int d = que.begin()->first;
    int v = que.begin()->second;
    que.erase(que.begin());
    if (d > pot2[v]) continue;

    if (in_spt[v]) {
      if (d <= pot1[v]) return true;
    } else {
      rep (i, adj[v].size()) {
        int tw = adj[v][i].first;
        int td = d + adj[v][i].second;
        if (td < pot2[tw]) {
          que.insert(mp(td, tw));
          pot2[tw] = td;
        }
      }
    }
  }

  return false;
}

void solve() {
  int lb = 0, ub = P + 1;
  while (ub - lb > 1) {
    int md = (lb + ub) / 2;
    if (check(md)) lb = md;
    else ub = md;
  }
  if (lb == P) puts("Looks Good To Me");
  else printf("%d\n", C[lb] + 1);
}










////////////////////////////////////////////////////////////////////////////////////////////////////
// Template
////////////////////////////////////////////////////////////////////////////////////////////////////

char out_buf[MAX_OUT], *out_p;

solver() : out_p(NULL) {}



void printf(const char* format, ...)
  __attribute__((format(printf, 2, 3)))
{
  if (!out_p) out_p = out_buf;

  va_list args;
  va_start(args, format);
  out_p += vsnprintf(out_p, sizeof(char) * (MAX_OUT - (out_p - out_buf)), format, args);
  va_end(args);

  if (out_p - out_buf >= MAX_OUT) {
    fprintf(stderr, "error: Output Limit Exceeded !!\n");
    exit(EXIT_FAILURE);
  }
}

void puts(const char *s) {
  printf("%s\n", s);
}

void debug(const char* format, ...)
  __attribute__((format(printf, 2, 3)))
{
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fflush(stderr);
}



END_SOLVER /////////////////////////////////////////////////////////////////////////////////////////



double sec() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}



void print_status(int c, int C, double t0, double t1, int nth) {
  static const int L = 5;
  if (C > L && c % (C / L) != 0) return;
  else if (c - (nth - 1) / 2 <= 0) fprintf(stderr, "[ case: %d / %d ]\n", c, C);
  else {
    double t = sec();
    fprintf(stderr, "[ case: %d / %d | time: %.3f / %.3f ]\n",
            c, C, t - t0, (t1 - t0) + (t - t1) / (c - (nth - 1) / 2) * C);
  }
}



int main(int argc, char **argv) {
  bool parallel = false, status = false;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) parallel = status = true;
    else if (strcmp(argv[i], "-s") == 0) status = true;
    else {
      fprintf(stderr, "usage: %s [-p] [-s]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  double t0 = sec();
  init();
  double t1 = sec();
  if (status) fprintf(stderr, "[ init: %.3f ]\n", t1 - t0);

  string tmp;
  getline(cin, tmp);
  int C = atoi(tmp.c_str());

  if (!parallel) {
    if (status) fprintf(stderr, "[ mode: single thread ]\n");

    rep (c, C) {
      if (status) print_status(c, C, t0, t1, 1);

      solver *s = new solver();
      assert(s != NULL);
      s->input();
      s->solve();
      printf(OUTPUT_FORMAT, c + 1, s->out_buf);
      fflush(stdout);
      delete s;
    }
  }
  else {
#ifdef _OPENMP

    int nth = omp_get_max_threads();
    if (status) fprintf(stderr, "[ mode: parallel (%d) ]\n", nth);

    vector<string> out(C);
    vector<bool> done(C);
    int solve_c = 0, out_c = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);

#pragma omp parallel for schedule(dynamic, 1)
    rep (c, C) {
      solver *s = new solver();
      assert(s != NULL);

      int my_c;
      omp_set_lock(&lock);
      {
        while (out_c < C && done[out_c]) {
          printf(OUTPUT_FORMAT, out_c + 1, out[out_c].c_str());
          fflush(stdout);
          out_c++;
        }
        if (status) print_status(solve_c, C, t0, t1, nth);
        my_c = solve_c++;
        s->input();
      }
      omp_unset_lock(&lock);

      s->solve();

      omp_set_lock(&lock);
      {
        out[my_c] = string(s->out_buf);
        done[my_c] = true;
      }
      omp_unset_lock(&lock);
      delete s;
    }

    omp_destroy_lock(&lock);

    while (out_c < C) {
      assert(done[out_c]);
      printf(OUTPUT_FORMAT, out_c + 1, out[out_c].c_str());
      out_c++;
    }

#else
    fprintf(stderr, "error: compile with -fopenmp !!\n");
#endif
  }

  exit(EXIT_SUCCESS);
}
