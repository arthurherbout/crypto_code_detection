#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef signed long long int64;
typedef unsigned long long uint64;

// Copyright (c) Derek Kisman (SnapDragon)  dkisman@gmail.com
// You must ask permission to use my BigInt code in your contests
const string DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
template<int RADIX, int CHUNKRADIX=RADIX, int CHUNKSIZE=1, class CT=char>
struct BigInt {
  vector<CT> ch;
  bool neg;

  BigInt& operator+=(const BigInt& b) {
    int car = 0; unsigned x;
    for (int i = 0; i < ch.size() || i < b.ch.size(); i++) {
      x = CHUNKRADIX + car + ((i<ch.size()) ? ch[i] : 0);
      if (neg == b.neg) x += (i<b.ch.size()) ? b.ch[i] : 0;
      else x -= (i<b.ch.size()) ? b.ch[i] : 0;
      if (i == ch.size()) ch.push_back(x%CHUNKRADIX);
      else ch[i] = x%CHUNKRADIX;
      car = (x/CHUNKRADIX)-1;
    }
    if (car == -1) {
      neg = !neg;
      auto it = ch.begin();
      for (car = 0; it != ch.end(); it++) {
        x = car + CHUNKRADIX - *it;
        *it = x%CHUNKRADIX;
        car = (x/CHUNKRADIX)-1;
      }
    } else if (car) ch.push_back(car);
    trim();
    return *this;
  }
  inline BigInt operator+(const BigInt& b) const {return BigInt(*this)+=b;}
  inline BigInt& operator-=(const BigInt& b) {return *this += -b;}
  inline BigInt operator-(const BigInt& b) const {return BigInt(*this)-=b;}
  BigInt& operator*=(const BigInt& b) {
    BigInt b2(*this); *this = BigInt();
    vector<CT> zeros;
    for (auto it = b.ch.begin(); it != b.ch.end(); it++) {
      BigInt b3 = b2 * *it;
      b3.ch.insert(b3.ch.begin(), zeros.begin(), zeros.end());
      *this += b3;
      zeros.push_back(0);
    }
    if (b.neg && !zero()) neg = !neg;
    return *this;
  }
  inline BigInt operator*(const BigInt& b) const {return BigInt(*this)*=b;}
  BigInt& operator/=(BigInt b) {
    bool retneg = (b.neg) ? !neg : neg;
    b.neg = neg = false;
    if (b > *this || b.zero()) return (*this=BigInt());
    if (b.ch.size() == 1) return (*this /= retneg ? -b.ch[0] : b.ch[0]);
    if (CHUNKRADIX >= (1<<21)) {
      int bm = CHUNKRADIX/b.ch.back() + 1; b *= bm; *this *= bm;
    }
    int64 p, q = (int64)CHUNKRADIX*b.ch.back() + b.ch[b.ch.size()-2];
    vector<CT> ret(ch.size()-b.ch.size()+1);
    b.ch.insert(b.ch.begin(), ret.begin(), ret.end());
    for (int i = ret.size()-1, j = ch.size()-1; i >= 0; i--, j--) {
      b.ch.erase(b.ch.begin());
      if (j >= ch.size()) {ret[i] = 0; continue;}
      p = (int64)CHUNKRADIX*ch[j] + ch[j-1] + 1;
      if (j+1 < ch.size()) p += (int64)CHUNKRADIX*CHUNKRADIX*ch[j+1];
      *this -= b*(ret[i] = p/q);
      while (neg) {*this += b; ret[i]--;}
    }
    ch = ret; neg = retneg;
    trim();
    return *this;
  }
  inline BigInt operator/(const BigInt& b) const {return BigInt(*this)/=b;}
  inline BigInt& operator%=(const BigInt& b) {return *this -= (*this/b)*b;}
  inline BigInt operator%(const BigInt& b) const {return BigInt(*this)%=b;}
  BigInt& operator*=(int x) {
    if (!x || zero()) return (*this = BigInt());
    neg ^= (x<0);
    uint64 car = 0, xl = abs(x); if (xl == 1) return *this;
    for (auto it = ch.begin(); it != ch.end(); it++) {
      car += *it * xl;
      *it = car%CHUNKRADIX;
      car /= CHUNKRADIX;
    }
    while (car) {ch.push_back(car%CHUNKRADIX); car /= CHUNKRADIX;}
    return *this;
  }
  inline BigInt operator*(int x) const {return BigInt(*this)*=x;}
  BigInt& operator/=(int x) {
    if (!x) {return *this = BigInt();}
    neg ^= (x<0);
    uint64 car = 0; x = abs(x); if (x == 1) return *this;
    for (auto it = ch.rbegin(); it != ch.rend(); it++) {
      car = CHUNKRADIX*car + *it;
      *it = car / x;
      car %= x;
    }
    trim();
    return *this;
  }
  inline BigInt operator/(int x) const {return BigInt(*this)/=x;}
  int operator%(int x) const {
    int ret = 0; x = abs(x);
    auto it = ch.rbegin();
    if (CHUNKRADIX%x == 0 && ch.size()) {it = --ch.rend();}
    for (; it != ch.rend(); it++) ret = ((uint64)CHUNKRADIX*ret + *it)%x;
    return neg ? -ret : ret;
  }

  bool operator<(const BigInt& b) const {
    if (neg != b.neg) return neg;
    if (ch.size() != b.ch.size()) return neg ^ (ch.size() < b.ch.size());
    for (int i = ch.size()-1; i >= 0; i--)
      if (ch[i] != b.ch[i]) return neg ^ (ch[i] < b.ch[i]);
    return false;
  }
  bool operator<=(const BigInt& b) const {
    if (neg != b.neg) return neg;
    if (ch.size() != b.ch.size()) return neg ^ (ch.size() < b.ch.size());
    for (int i = ch.size()-1; i >= 0; i--)
      if (ch[i] != b.ch[i]) return neg ^ (ch[i] < b.ch[i]);
    return true;
  }
  inline bool operator==(const BigInt& b) const
    {return neg==b.neg && ch==b.ch;}
  inline bool operator>(const BigInt& b) const {return !(*this <= b);}
  inline bool operator>=(const BigInt& b) const {return !(*this < b);}
  inline bool operator!=(const BigInt& b) const {return !(*this == b);}
  friend inline BigInt operator-(const BigInt& b)
    {BigInt ret(b); ret.neg = !ret.neg; return ret;}
  friend inline BigInt abs(const BigInt& b) {return b.neg ? -b : b;}

  size_t digits() const {
    if (ch.size() == 0) return 1;
    size_t ret = (ch.size()-1)*CHUNKSIZE;
    for (int x = ch.back(); x; x /= RADIX) ret++;
    return ret;
  }
  inline bool zero() const {return ch.size() == 0;}
  int64 toint() const {
    int64 ret = 0;
    for (auto it = ch.rbegin(); it != ch.rend(); it++)
      ret = ret*CHUNKRADIX + *it;
    return neg ? -ret : ret;
  }
  string tostring() const {
    if (ch.size() == 0) return "0";
    auto it = ch.begin();
    string ret;
    int i, x;
    while (it != ch.end()) {
      x = *it;
      if (++it == ch.end()) break;
      for (i = 0; i < CHUNKSIZE; i++, x /= RADIX)
        ret += DIGITS[x%RADIX];
    }
    for (; x; x /= RADIX) ret += DIGITS[x%RADIX];
    if (neg) ret += '-';
    reverse(ret.begin(), ret.end());
    return ret;
  }
  friend inline ostream& operator<<(ostream& o, const BigInt& b)
    {return o << b.tostring();}
  friend inline istream& operator>>(istream& i, BigInt& b)
    {string s; i >> s; b = BigInt(s); return i;}
  inline void trim() {
    while (ch.size() && !ch.back()) ch.pop_back();
    if (zero()) neg = false;
  }

  inline BigInt() {neg = false;}
  inline BigInt(const BigInt& b) {ch = b.ch; neg = b.neg;}
  template<int R, int CR, int CS, class T>
  BigInt(const BigInt<R,CR,CS,T>& b) {
    if (b.zero()) {*this = BigInt(); return;}
    neg = false;
    BigInt p(1);
    for (int i = 0; i < b.ch.size(); i++) {
      *this += p*b.ch[i];
      p *= CR;
    }
    neg = b.neg;
  }
  BigInt(int64 n) {
    if ((neg = (n < 0))) n = -n;
    for (; n; n /= CHUNKRADIX) ch.push_back(n%CHUNKRADIX);
  }
  inline BigInt(int n) {*this = BigInt((int64)n);}
  BigInt(const char *s) {
    const char *it, *it2 = s-1;
    if ((neg = (*s=='-'))) it2++;
    int x = 0, y = 1, z;
    for (it = s; *it; it++);
    for (--it; it != it2; --it) {
      z = DIGITS.find(toupper(*it));
      if (z < 0 || z >= RADIX) {ch.clear(); x = 0; y = 1; continue;}
      x += y*z; y *= RADIX;
      if (y == CHUNKRADIX) {ch.push_back(x); x = 0; y = 1;}
    }
    if (y > 1) ch.push_back(x);
    trim();
  }
  inline BigInt(const string& s) {*this = BigInt(s.c_str());}
};

typedef BigInt<10, 1000000000, 9, int> bigint;
template<typename T> T Gcd(const T& a, const T& b) {
  return b != T() ? Gcd(b, a%b) : abs(a);
}

int main() {
  int T, prob=1;
  for (cin >> T; T--;) {
    int S, N;
    cin >> S >> N;
    vector<pair<char, bigint>> C(N);
    for (int i = 0; i < N; i++) {
      cin >> C[i].first >> C[i].second;
      if (C[i].first == '+' && C[i].second < 0) {
        C[i].second = -C[i].second;
        C[i].first = '-';
      }
      if (C[i].first == '-' && C[i].second < 0) {
        C[i].second = -C[i].second;
        C[i].first = '+';
      }
    }

    bigint pos = 0, neg = 0;
    for (int i = 0; i < C.size(); i++) if (C[i].first == '+') pos += C[i].second;
    for (int i = 0; i < C.size(); i++) if (C[i].first == '-') neg += C[i].second;
    for (int i = 0; i < C.size(); i++) {
      if (C[i].first == '+' || C[i].first == '-') {
        C.erase(C.begin()+i--);
      }
    }
    if (pos != 0) C.emplace_back('+', pos);
    if (neg != 0) C.emplace_back('-', neg);

    int minmi = -1, maxmi = -1, mindi = -1, maxdi = -1;
    for (int i = 0; i < C.size(); i++) {
      if (C[i].first == '*' && C[i].second < 0) {
        if (minmi == -1 || -C[i].second < -C[minmi].second) minmi = i;
        if (maxmi == -1 || -C[i].second > -C[maxmi].second) maxmi = i;
      }
      if (C[i].first == '/' && C[i].second < 0) {
        if (mindi == -1 || -C[i].second < -C[mindi].second) mindi = i;
        if (maxdi == -1 || -C[i].second > -C[maxdi].second) maxdi = i;
      }
    }

    vector<pair<char, bigint>> C2;
    bigint mul = 1, div = 1;
    int nz = 0;
    for (int i = 0; i < C.size(); i++) {
      if (i != minmi && i != mindi) {
        if (C[i].first == '*' && C[i].second == 0) {
          nz++;
        } else if (C[i].first == '*') {
          mul *= C[i].second;
        } else if (C[i].first == '/') {
          div *= C[i].second;
        } else {
          C2.push_back(C[i]);
        }
      } else {
        C2.push_back(C[i]);
      }
    }
    C.swap(C2);
    if (mul != 1) C.emplace_back('*', mul);
    if (div != 1) C.emplace_back('/', div);
    if (nz) C.emplace_back('*', 0);

// Now I have a major problem...
// 9! is probably too large for all the bigint crap I'm doing.
// Ok, I haven't really proved this is good enough ... but it looks like 7! will work.  So, yay.

/*if (prob == 14 || prob == 16) {
cerr << S << endl;
for (int i = 0; i < C.size(); i++) cerr << ' ' << C[i].first << ' ' << C[i].second << endl;
}*/

    sort(C.begin(), C.end());
    bool first = true;
    bigint retn, retd;
    do {
      bigint curn = S, curd = 1;
      for (int i = 0; i < C.size(); i++) {
        if (C[i].first == '+') { curn += curd * C[i].second; }
        else if (C[i].first == '-') { curn -= curd * C[i].second; }
        else if (C[i].first == '*') { curn *= C[i].second; }
        else if (C[i].first == '/') { curd *= C[i].second; }
      }
      if (curd < 0) { curn = -curn; curd = -curd; }
      if (first || curn * retd > retn * curd) { retn = curn; retd = curd; }
      first = false;
    } while (next_permutation(C.begin(), C.end()));

    bigint g = Gcd(retn, retd);
    cout << "Case #" << prob++ << ": " << retn / g << ' ' << retd / g << endl;
  }
}
