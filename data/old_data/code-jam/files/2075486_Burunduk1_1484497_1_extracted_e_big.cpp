/**
 * Author: Sergey Kopeliovich (Burunduk30@gmail.com)
 * Date: 2012.07.27
 */

/* Template */

#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <fstream>
#include <queue>
#include <bitset>
#include <numeric>
#include <iterator>

using namespace std;

#ifdef _WIN32
#  define I64 "%I64d"
#else
#  define I64 "%lld"
#endif

#define forn(i, n) for (int i = 0; i < (int)(n); i++)
#define fornd(i, n) for (int i = (int)(n) - 1; i >= 0; i--)
#define forab(i, a, b) for (int i = (int)(a); i <= (int)(b); i++)
#define forabd(i, a, b) for (int i = (int)(b); i >= (int)(a); i--)
#define forit(i, a) for (__typeof((a).begin()) i = (a).begin(); i != (a).end(); i++)
#define sz(a) (int)(a).size()
#define all(a) (a).begin(), (a).end()
#define zero(a) memset(a, 0, sizeof(a))
#define Fill(a, x) memset(a, x, sizeof(a))
#define pb push_back
#define mp make_pair
#define uni(a) (unique(all(a)) - (a).begin());
#define BS(a, x, less) (lower_bound(all(a), x, less) - (a).begin())

#define EOL(i, n) " \n"[i == (n) - 1]
#define LEN(a) (int)(sizeof(a) / sizeof(a[0]))
#define IS(a, i) (((a) >> (i)) & 1)
#define ONE(n) ((1 << (n)) - 1)
#define LAST(a) (a)[sz(a) - 1]
#define LASTC(s) (s)[strlen((s)) - 1]

typedef short int16;
typedef unsigned short uint16;
typedef unsigned int uint;
typedef double dbl;
typedef long long ll;
typedef unsigned long long ull;
typedef vector <int> vi;
typedef vector <vi> vvi;
typedef vector <string> vs;
typedef pair <int, int> pii;
typedef vector <pii> vpii;
typedef unsigned char byte;

template <class T> T inline sqr(T x) { return x * x; }
template <class T> inline void relaxMin( T &a, T b ) { a = min(a, b); }
template <class T> inline void relaxMax( T &a, T b ) { a = max(a, b); }
template <class T> inline T sign( T x ) { return x > 0 ? 1 : (x < 0 ? -1 : 0); }
template <class T> inline T myAbs( T a ) { return a > 0 ? a : -a; }
template <class T> T iteratorK( T a, int k ) { while (k--) a++; return a; }

#define DEBUG 1
void err( const char *fmt, ... )
{
  #if DEBUG
  va_list list;
  va_start(list, fmt);
  vfprintf(stderr, fmt, list);
  fflush(stderr);
  #endif
}

void Assert( bool f, const char *fmt = "", ... )
{
  if (!f)
  {
    va_list list;
    va_start(list, fmt);
    err(fmt, list);
    exit(1);
  }
}

unsigned R() { return (rand() << 15) + rand(); }

double start = clock();
void TimeStamp( const char *s = 0, ... )
{
  va_list list;
  va_start(list, s);
  if (s)
  {
    fprintf(stderr, "time = %.2f : ", (clock() - start) / CLOCKS_PER_SEC);
    vfprintf(stderr, s, list);
    fprintf(stderr, "\n");
  }
  else
    fprintf(stderr, "time = %.2f\n", (clock() - start) / CLOCKS_PER_SEC);
  start = clock();
}

void MiniTimeStamp( const char *s )
{
  fprintf(stderr, "time = %.2f : %s\n", (double)(clock() - start) / CLOCKS_PER_SEC, s);
}

/* The main part */

const int maxN = 50;
const int MAX = 20;

int n, x[maxN][2];
ll cnt;
int was[maxN], no[maxN];
int k, ind[maxN];
int bad;
int cc, used[2 * MAX][1 << MAX];
int last[2 * MAX][1 << MAX];
int add_num[2 * MAX][1 << MAX];
int change[2 * MAX][1 << MAX];
int pos;
int tim;

int calc()
{
  tim++;
  if (bad)
    return 0;

  int h = 0, cpos = pos;
  forn(i, k)
    h |= IS(cnt, ind[i]) << i;
  if (used[cpos][h] == cc)
    return h;
  if (used[cpos][h] == cc + 1)
  {
    bad = 1;
    return h;
  }
  used[cpos][h] = cc + 1;

  int opos = pos;
  pos = x[pos][IS(cnt, pos)];
  cnt ^= 1LL << opos;

  int gpos = pos;
  if (!was[gpos])
  {
    last[cpos][h] = gpos;
    add_num[cpos][h] = 1;
    change[cpos][h] = 1 << no[cpos];
  }
  else
  {      
    int h1 = h ^ (1 << no[cpos]);
    calc();
    last[cpos][h] = last[gpos][h1];
    add_num[cpos][h] = 1 + add_num[gpos][h1];
    change[cpos][h] = (1 << no[cpos]) ^ change[gpos][h1];
  }

  used[cpos][h] = cc;
  return h;
}

int main()
{
#ifdef DEBUG
  double tmp_start = clock();
  fprintf(stderr, "Start\n");
#endif

  int tn;
  scanf("%d", &tn);
  forn(tt, tn)
  {
    fprintf(stderr, "Case #%d: ", tt + 1);
    printf("Case #%d: ", tt + 1);

    tim = 0;
    cc += 2;
//    memset(used, 0, sizeof(used));

    scanf("%d", &n);
    forn(i, n - 1)
      forn(j, 2)
        scanf("%d", &x[i][j]), x[i][j]--;
    cnt = 0;

    int M = 1 << MAX;
    ll num = 0, all = 1LL << n;
    pos = 0;

    while (pos != n - 1 && num < M)
    {
      tim++;
      num++;
      int opos = pos;
      pos = x[pos][IS(cnt, pos)];
      cnt ^= 1LL << opos;
    }

    err("[%d]", num);
    
    Fill(was, 0);
    k = 0;
    while (k < MAX && pos != n - 1 && num <= 2 * M)
    {
      tim++;
      if (!was[pos])
        no[pos] = k, was[pos] = 1, ind[k++] = pos;
      num++;
      int opos = pos;
      pos = x[pos][IS(cnt, pos)];
      cnt ^= 1LL << opos;
    }

    err("[%d]", num);

//    err("k = %d, MAX = %d\n", k, MAX);
    
    bad = 0;
    if (pos != n - 1 && num <= 2 * M) // maybe not infty
    {
      while (!bad && pos != n - 1 && num <= all)
      {
        tim++;
        if (!was[pos])
        {
          num++;
          pos = x[pos][IS(cnt, pos)];
          cnt ^= 1LL << pos;
        }
        else
        {
//          fprintf(stderr, "!");
          int cpos = pos, h = calc();
//          fprintf(stderr, "!");
          pos = last[cpos][h];
          num += add_num[cpos][h];
          int ch = change[cpos][h];
          forn(i, k)
            if (IS(ch, i))
              cnt ^= 1LL << ind[i];
        }
      }
    }

    fprintf(stderr, "%d\n", tim);

    if (pos == n - 1)
      printf("%I64d\n", num);
    else
      puts("Infinity");
  }

#ifdef DEBUG
  fprintf(stderr, "Total time = %.2f\n", (clock() - tmp_start) / CLOCKS_PER_SEC);
#endif
  return 0;
}
