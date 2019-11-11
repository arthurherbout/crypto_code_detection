#include <algorithm>
#include <string>
#include <vector>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
//#include <iostream>
#include <set>
//#include <map>
//#include <sstream>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<int> vll;
typedef vector<string> vs;

int err;

#define LS <
#define Size(x) (int(x.size()))
#define LET(k,val) typeof(val) k = (val)
#define CLC(act,val) (*({act; static typeof(val) CLCR; CLCR = (val); &CLCR;}))

#define FOR(k,a,b) for(typeof(a) k=(a); k LS (b); ++k)
#define FORREV(k,a,b) for(typeof(b) k=(b); (a) <= (--k);)

#define FIRST(k,a,b,cond) CLC(LET(k, a); for(; k LS (b); ++k) if(cond) break, k)
#define LAST(k,a,b,cond) CLC(LET(k, b); while((a) <= (--k)) if(cond) break, k)
#define EXISTS(k,a,b,cond) (FIRST(k,a,b,cond) LS (b))
#define FORALL(k,a,b,cond) (!EXISTS(k,a,b,!(cond)))
#define FOLD0(k,a,b,init,act) CLC(LET(k, a); LET(R##k, init); for(; k LS (b); ++k) {act;}, R##k)
#define SUMTO(k,a,b,init,x)  FOLD0(k,a,b,init,R##k += (x))
#define SUM(k,a,b,x) SUMTO(k,a,b,(typeof(x)) (0), x)
#define PRODTO(k,a,b,init,x) FOLD0(k,a,b,init,R##k *= (x))
#define PROD(k,a,b,x) PRODTO(k,a,b,(typeof(x)) (1), x)
#define MAXTO(k,a,b,init,x)  FOLD0(k,a,b,init,R##k >?= (x))
#define MINTO(k,a,b,init,x)  FOLD0(k,a,b,init,R##k <?= (x))
#define QXOR(k,a,b,x) FOLD0(k,a,b,(typeof(x)) (0), R##k ^= x)
#define QAND(k,a,b,x) FOLD0(k,a,b,(typeof(x)) (-1), R##k &= x)
#define QOR(k,a,b,x) FOLD0(k,a,b,(typeof(x)) (-1), R##k |= x)
#define FOLD1(k,a,b,init,act) CLC(LET(k, a); LET(R##k, init); for(++k; k LS (b); ++k) act, R##k)
#define MAX(k,a,b,x) FOLD1(k,a,b,x, R##k >?= (x))
#define MIN(k,a,b,x) FOLD1(k,a,b,x, R##k <?= (x))
#define FIRSTMIN(k,a,b,x) (MIN(k,a,b,make_pair(x,k)).second)

int bitc(ll r) {return r == 0 ? 0 : (bitc(r>>1) + (r&1));}
ll gcd(ll x, ll y) {return x ? gcd(y%x,x) : y;}

// template<class T> T& operator >?= (T& x, T y) {if(y>x) x=y; return x;}
// template<class T> T& operator <?= (T& x, T y) {if(y<x) x=y; return x;}
// template<class T> T operator >? (T x, T y) {return x>y?x:y;}
// template<class T> T operator <? (T x, T y) {return x<y?x:y;}

#define Pa(xy) ((xy).first)
#define Ir(xy) ((xy).second)

string cts(char c) {string s=""; s+=c; return s;}

/// ----

#define BUFSIZE 1000000
char buf[BUFSIZE];

#ifdef DEBUG
#define DEB(x) x
#else
#define DEB(x)
#endif

string getLine() {
  string s;
  while(!feof(stdin)) {
    char c = fgetc(stdin);
    if(c == 13) continue;
    if(c == 10) return s;
    s += c;
    }
  return s;
  }

int getNum() {
  string s = getLine();
  return atoi(s.c_str());
  }

vi parsevi(string s) {
  s = s + " ";
  int q = 0;
  bool minus = false;
  vi res;
  FOR(l,0, Size(s)) {
    if(s[l] == ' ') { res.push_back(minus?-q:q); q = 0; minus = false;}
    else if(s[l] == '-') { minus = true; }
    else { q = q * 10 + s[l] - '0'; }
    }
  return res;
  }

int Tests, cnum;

//Eryx

// !FDI

int N;
int gold[5000];

vector<int> nei[5000];

int testOut(int at, int from) {
  int s = gold[at];
  FOR(u, 0, Size(nei[at])) {
    int v = nei[at][u];
    if(v != from) s = max(s, gold[at] + testOut(v, at));
    }
  return s;  
  }

int testDefend(int wa, int wb, int ba, int bb, int bc, int *whA, int *whB) {
  int bs = 0;
  if(wb == wa) {
    // go somewhere else
    FOR(u, 0, Size(nei[wb])) {
      int v = nei[wb][u];
      if(v != ba && v != bb && v != bc) {
        int sc = testOut(v, wb);
        if(sc>bs) bs = sc;
        }
      }
    }

  if(wb != wa) {
    // go towards wa
    int sc = gold[wb] + testDefend(wa, whA[wb], ba, wb, bc, whA, whB);
    if(sc>bs) bs = sc;
    // go not towards wa
    FOR(u, 0, Size(nei[wb])) {
      int v = nei[wb][u];
      if(v != bb && v != whA[wb]) {
        int sc = gold[wb] + testOut(v, wb);
        if(sc>bs) bs = sc;
        }
      }
    }

  return bs;
  }

int testBoth(int wa, int wb, int ba, int bb, int *whA, int *whB) {
  // go towards enemy
  int sc = -2000000000;
  
  if(wa != wb)
    sc = gold[wa] - testBoth(wb, whB[wa], bb, wa, whB, whA);

  // go another direction
  FOR(u, 0, Size(nei[wa])) {
    int v = nei[wa][u];
    if(v != ba && v != whB[wa]) {
      int sc2 = gold[wa] + testOut(v, wa) - testDefend(wa, wb, ba, bb, v, whA, whB);
      if(sc2 > sc) sc = sc2;
      }
    }
  
  // stay in place
  int sc2 = gold[wa] - testDefend(wa, wb, ba, bb, -1, whA, whB);
  if(sc2 > sc) sc = sc2;

  return sc;
  }

int whereisA[5000], whereisB[5000], whereA, bscore, lscore;

void findB(int at, int from) {
  whereisA[at] = from; whereisB[at] = -1;
  int scor = testBoth(whereA, at, -1, -1, whereisA, whereisB);
  // printf("%d,%d => %d\n", whereA, at, scor);
  if(scor < lscore) lscore = scor;
  FOR(u, 0, Size(nei[at])) if(nei[at][u] != from) {
    whereisB[at] = nei[at][u];
    findB(nei[at][u], at);
    }
  }
  
void findA(int at, int from) {
  whereA = at; 
  lscore = 2000000000;
  findB(at, -1);
  if(lscore > bscore) bscore = lscore;
  FOR(u, 0, Size(nei[at])) if(nei[at][u] != from)
    findA(nei[at][u], at);
  }

void solveCase() {
  int res = 0;
  
  err=scanf("%d", &N);
  FOR(n,0,N) err=scanf("%d", &gold[n]);
  
  FOR(n,0,N) nei[n].clear();
  
  FOR(n,0,N-1) {
    int ch;
    err=scanf("%d", &ch);
    
    nei[n].push_back(ch-1);
    nei[ch-1].push_back(n);
    }
  
  bscore = -2000000000;
  
  findA(0, -1);
  
  printf("Case #%d: %d\n", cnum, bscore);
  }

int main() {

  if(0)
    Tests = 1;
  else if(1)
    err = scanf("%d", &Tests);
  else {
    string Nstr = getLine();
    Tests = atoi(Nstr.c_str());
    }
  
  for(cnum=1; cnum<=Tests; cnum++)
    solveCase();
    
  // finish
  return 0;
  }

// This solution includes hidden routines to solve test cases in separate
// processes in order to make it faster. I will update them to run on a
// cluster if I get one ;)
