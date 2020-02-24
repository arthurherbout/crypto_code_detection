//    g++ -Wall -O3 main.cpp -lgmpxx

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>  // What's our vector, Victor?

#include <gmpxx.h>  // see <http://gmplib.org/>

using namespace std;  // everybody has AIDS

// mpz_class? wtf is GNU thinking?  oh wait, gnamespaces.
typedef mpz_class bigint;
typedef long long ll;
typedef long double ld;
typedef string str;  // my precious

// What's our vec, Vic?
#define vec vector

#define It iterator

// never going to use this
#define SZ(x) ((int)((x).size()))

// forgot about this for a while
template <class T, class U>
pair<T, U> tup(T const& x, U const& y) {
  return make_pair(x, y);
}

/*
template <class iter>
int lexicographic_cmp(iter x_beg, iter x_end, iter y_beg, iter y_end) {
  for (;;) {
    if (x_beg == x_end) {
      return y_beg == y_end ? 0 : -1;
    }
    if (y_beg == y_end) {
      return 1;
    }

    typename iterator_traits<iter>::value_type const& xval = *x_beg;
    typename iterator_traits<iter>::value_type const& yval = *y_beg;

    if (xval < yval) {
      return -1;
    }
    if (yval < xval) {
      return 1;
    }
    ++x_beg;
    ++y_beg;
  }
  return 0;
}
// */

template <class iter>
typename iterator_traits<iter>::value_type sum_elements(iter beg, iter end) {
  typename iterator_traits<iter>::value_type r(0);
  while (beg != end) {
    r += *beg;
    ++beg;
  }
  return r;
}

template <class TInt>
TInt gcd(TInt x, TInt y) {
  if (x == 0 && y == 0) {
    cerr << "warning: computing gcd(0, 0) => 0" << endl;
  }
  
  y = abs(y);
  x = abs(x);
  while (y != 0) {
    TInt tmp = x % y;
    x = y;
    y = tmp;
  }
  return x;
}

bigint gcd(bigint x, bigint y) {
  return gcd<bigint>(x, y);
}

ll gcd(ll x, ll y) {
  return gcd<ll>(x, y);
}

int gcd(int x, int y) {
  return gcd<int>(x, y);
}

bigint lcm(bigint x, bigint y) {
  return x * y / gcd(x,y);
}

#define upto(fvar, flo, fhi) for (typeof(flo) fvar = flo; fvar < fhi; ++fvar)
#define from(fvar, flo, fhi) for (typeof(flo) fvar = flo; fvar <= fhi; ++fvar)

// i am never going to use this
#define fdown(fvar, fhi, flo) for (typeof(fhi) fvar = fhi; fvar-- > flo;)

template <class T>
vector<T> VEC(int count) {
  vector<T> ret;
  ret.reserve(count);
  upto(i,0,count) {
    T x;
    cin >> x;
    ret.push_back(x);
  }
  return ret;
}

int INT() {
  int x;
  if (!(cin >> x)) {
    cin.clear();
    str ln;
    getline(cin, ln);
    throw runtime_error("INT() error (ln is " + ln);
  }

  return x;
}

double DOUBLE() {
  double x;
  if (!(cin >> x)) {
    throw runtime_error("DOUBLE() error");
  }
  return x;
}

ll LONGLONG() {
  ll x;
  if (!(cin >> x)) {
    throw runtime_error("LONGLONG() error");
  }
  return x;
}

bigint BIGINT() {
  bigint x;
  if (!(cin >> x)) {
    throw runtime_error("BIGINT() error");
  }
  return x;
}

str LINE() {
  str x;
  if (!getline(cin, x)) {
    throw runtime_error("LINE() error");
  }
  return x;
}

vec<str> LINES(int n) {
  vec<str> ret;
  upto(i,0,n) {
    ret.push_back(LINE());
  }
  return ret;
}

void CHAR(char expected) {
  char c;
  cin >> c;
  if (c != expected) {
    throw runtime_error(str("CHAR() expecting c == '") + expected + str("' but got '") + c + str("'"));
  }
}

string WORD() {
  string s;
  cin >> s;
  return s;
}

// iostreams :(
std::ostream& FLOUT(std::ostream& out, double d) {
  return cout << fixed << setprecision(9) << d;
}

void FLOUT(double d) {
  FLOUT(cout, d);
}

/*
struct hhmm {
  int h, m;
  hhmm(int h_, int m_) : h(h_), m(m_) { }
  int mins() const {
    return h * 60 + m;
  }
};

// just in case :)
hhmm HHMM() {
  int h = INT();
  CHAR(':');
  int m = INT();
  return hhmm(h, m);
}
// */



/*
// enter the matrix

template <class T>
struct matrix {
  int R, C;
  vector<T> vals;

  explicit matrix(int n) : R(n), C(n), vals(n * n, T(0)) { }
  matrix(int r, int c) : R(r), C(c), vals(r * c, T(1)) { }

  static matrix iden(int n) {
    matrix ret(n);
    for (int i = 0; i < n; ++i) {
      ret.at(i, i) = 1;
    }
    return ret;
  }


  inline T& at(int r, int c) {
    return vals[r*C + c];
  }
  inline T const& at(int r, int c) const {
    return vals[r*C + c];
  }

  runtime_error size_mismatch(const char *operation, const matrix<T>& rhs) const {
    stringstream msg("size mismatch doing ");
    msg << operation << ": (" << R << "," << C << ") vs (" << rhs.R << "," << rhs.C << ")";
    return runtime_error(msg.str());
  }

  matrix& operator+=(const matrix& rhs) {
    if (R != rhs.R || C != rhs.C) {
      throw size_mismatch("operator+=", rhs);
    }

    typename vector<T>::iterator p = vals.begin(), pe = vals.end();
    typename vector<T>::const_iterator q = rhs.vals.begin();
    while (p != pe) {
      *p++ += *q++;
    }
    return *this;
  }

  matrix operator+(const matrix& rhs) const {
    matrix ret(*this);
    ret += rhs;
    return ret;
  }

  matrix& operator-=(const matrix& rhs) {
    if (R != rhs.R || C != rhs.C) {
      throw size_mismatch("operator-=", rhs);
    }
    typename vector<T>::iterator p = vals.begin(), pe = vals.end();
    typename vector<T>::const_iterator q = rhs.vals.begin();
    while (p != pe) {
      *p++ -= *q++;
    }
    return *this;
  }

  matrix operator-(const matrix& rhs) const {
    matrix ret(*this);
    ret -= rhs;
    return ret;
  }

  matrix operator*(const matrix<T>& rhs) const {
    if (C != rhs.R) {
      throw size_mismatch("operator*", rhs);
    }
    
    int rhsC = rhs.C;
    matrix<T> ret(R, rhsC);

    typename vector<T>::iterator out = ret.vals.begin();
    for (int i = 0; i < R; ++i) {
      typename vector<T>::const_iterator p_reset = vals.begin() + i*C;
      typename vector<T>::const_iterator pe = p_reset + C;
      for (int j = 0; j < rhsC; ++j) {
	typename vector<T>::const_iterator p = p_reset;
	typename vector<T>::const_iterator q = rhs.vals.begin() + j;

	T sum(0);
	while (p != pe) {
	  sum += (*p) * (*q);
	  q += rhsC;
	  ++p;
	}

	*out++ = sum;
      }
    }

    return ret;
  }

  matrix pow(long long p) {
    if (R != C) {
      throw size_mismatch("power", *this);
    }
    if (p < 0) {
      stringstream msg("matrix::power: negative power (");
      msg << p << ")";
      throw runtime_error(msg.str());
    }
    
    matrix ret = matrix::iden(R);
    matrix tmp(*this);

    while (p > 0) {
      if (p & 1) {
	ret = ret * tmp;
      }
      tmp = tmp * tmp;
      p >>= 1;
    }
    return ret;
  }


  std::ostream& print(std::ostream& out) const {
    for (int i = 0; i < R; ++i) {
      for (int j = 0; j < C; ++j) {
	out << at(i, j) << (j == C-1 ? '\n' : ' ');
      }
    }
    return out;
  }
};
// */

template <class T>
std::ostream& joinout(std::ostream& out, const vec<T>& obj, const string& s) {
  bool notfirst = false;
  for (typename std::vector<T>::const_iterator p = obj.begin(), e = obj.end();
       p != e; ++p) {
    if (notfirst) {
      out << s;
      notfirst = true;
    }
    out << *p;
    notfirst = true;
  }
  return out;  
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& obj) {
  return joinout(out, obj, " ");
}

template <class T, class U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& obj) {
  return out << "(" << obj.first << "," << obj.second << ")";
}






// unification
/*
vec<int> refs(1100000, -1);

int getreal(int i) {
  vector<int> rst;
  for(;;) {
    int r = refs[i];
    if (r == -1) {
      int sz = rst.size();
      for (int j = 0; j < sz; ++j) {
       refs[rst[j]] = i;
      }
      return i;
    }
    rst.push_back(i);
    i = r;
  }
}

void unify(int i, int j) {
  int reali = getreal(i);
  int realj = getreal(j);

  if (reali != realj) {
    refs[reali] = realj;
  }
}
// */

/*
// i am a bad person
void genprimes(vec<bool>& out, ll limit) {
  out.clear();
  out.resize(limit + 1, true);

  out[0] = false;
  out[1] = false;

  for (ll i = 2; i * i < limit; ++i) {
    if (out[i]) {
      for (ll j = i * i; j < limit; j += i) {
	out[j] = false;
      }
    }
  }
}

vec<string> strsplit(const string& s, char c) {
  vec<string> ret;

  int sz = s.size();
  string curr = "";
  for (int i = 0; i < sz; ++i) {
    char d = s[i];
    if (d == c) {
      ret.push_back(curr);
      curr = "";
    } else {
      curr.push_back(d);
    }
  }
  ret.push_back(curr);
  return ret;
}
// */





// -----------------
// TOPTOP



int hhh[5000000];

int lo, hi;

void state() {
  from(i,lo,hi) {
    int val  = hhh[2500000+i];
    if (val) {
      cerr << i << ": " << val << endl;
    }
  }
}


void run(int casenum) {
  // REMEMBER TO CLEAR
  upto(i,0,5000000) {
    hhh[i] = 0;
  }

  int *h = hhh + 2500000;

  int C = INT();

  lo = 5000000;
  hi = -5000000;

  upto(i,0,C) {
    int p = INT();
    int v = INT();
    h[p] = v;
    if (p < lo) {
      lo = p;
    }
    if (p > hi) {
      hi = p;
    }
  }

  cerr << "lo: " << lo << " hi: " << hi << endl;

  ll steps = 0;
  bool finished = true;
  for (;;) {
    finished = true;
    from(i,lo,hi) {
      if (h[i] > 1) {
	finished = false;
	break;
      }
    }
    if (finished) {
      break;
    }
    
    //    cerr << "state: " << steps << endl;
    //    state();

    int tmplo = lo;
    int tmphi = hi;
    from(i,tmplo,tmphi) {
      int newsteps = h[i] / 2;
      if (newsteps > 0) {
	lo = min(lo,i-1);
	hi = max(hi,i+1);
	h[i] -= (2 * newsteps);
	h[i-1] += newsteps;
	h[i+1] += newsteps;
	steps += newsteps;
      }
    }
  }


  cout << "Case #" << casenum << ": ";
  cout << steps;
  cout << endl;
}


int main() {
  int numcases = INT();
  LINE();
  cerr << "(numcases = " << numcases << ")\n";
  from(casenum, 1, numcases) {
    cerr << "(Computing case " << casenum << ")\n";

    run(casenum);
  }
}
