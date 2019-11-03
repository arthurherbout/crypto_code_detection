#include "assert.h"
#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "algorithm"
#include "numeric"
#include "functional"
#include "bitset"
#include "vector"
#include "list"
#include "set"
#include "map"
#include "queue"
#include "stack"
#include "string"
#include "sstream"
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;

//////////////////////////////////////////////////////////////////////////////////////////
struct BigInteger {
  vector<int> digits;
  static const int BASE, DIGITS;
  BigInteger(int value = 0);
  BigInteger(const char* str);
  int get(int k) const;
  void set(int k, int value);
  void normalize();
  string toString() const;
};

const int BigInteger::BASE = 10000;
const int BigInteger::DIGITS = 4;

BigInteger::BigInteger(int value) {
  for (; value > 0; value /= BASE)
    digits.push_back(value % BASE);
}
BigInteger::BigInteger(const char* str) {
  char buffer[DIGITS + 1]; memset(buffer, '\0', sizeof(buffer));
  for (int length = strlen(str); length > 0; length -= DIGITS) {
    const int digitSize = min(DIGITS, length);
    memcpy(buffer, &str[length - digitSize], digitSize * sizeof(char));
    buffer[digitSize] = '\0';
    digits.push_back(atoi(buffer));
  }
  normalize();
}

int BigInteger::get(int k) const {
  if (k >= (int)digits.size()) return 0;
  else return digits[k];
}
void BigInteger::set(int k, int value) {
  if (k >= (int)digits.size()) digits.resize(k + 1, 0);
  digits[k] = value;
}
void BigInteger::normalize() {
  while (!digits.empty() && digits.back() == 0)
    digits.pop_back();
}

string BigInteger::toString() const {
  if (digits.empty()) return string("0", 1);
  char buffer[DIGITS + 1];
  const int length = sprintf(buffer, "%d", digits.back());
  string str(buffer, length);
  for (int k = digits.size() - 2; k >= 0; --k) {
    sprintf(buffer, "%0*d", DIGITS, digits[k]);
    str.append(buffer, DIGITS);
  }
  return str;
}

BigInteger operator <<(const BigInteger& b1, int offset) {
  BigInteger b2; b2.digits.insert(b2.digits.end(), offset, 0);
  b2.digits.insert(b2.digits.end(), b1.digits.begin(), b1.digits.end());
  b2.normalize(); return b2;
}

BigInteger& operator <<=(BigInteger& b1, int offset) {
  b1.digits.insert(b1.digits.begin(), offset, 0);
  b1.normalize(); return b1;
}

BigInteger operator >>(const BigInteger& b1, int offset) {
  BigInteger b2; if (offset < (int)b1.digits.size())
    b2.digits.insert(b2.digits.begin(), b1.digits.begin() + offset, b1.digits.end());
  return b2;
}

BigInteger operator >>=(BigInteger& b1, int offset) {
  if ((int)b1.digits.size() <= offset) b1.digits.clear();
  else b1.digits.erase(b1.digits.begin(), b1.digits.begin() + offset);
  return b1;
}

bool operator <(const BigInteger& b1, const BigInteger& b2) {
  if (b1.digits.size() < b2.digits.size()) return true;
  if (b2.digits.size() < b1.digits.size()) return false;
  for (int k = b1.digits.size() - 1; k >= 0; --k) {
    if (b1.digits[k] < b2.digits[k]) return true;
    if (b2.digits[k] < b1.digits[k]) return false;
  }
  return false;
}

bool operator >(const BigInteger& b1, const BigInteger& b2) {return b2 < b1;}
bool operator <=(const BigInteger& b1, const BigInteger& b2) {return !(b2 < b1);}
bool operator >=(const BigInteger& b1, const BigInteger& b2) {return !(b1 < b2);}
bool operator ==(const BigInteger& b1, const BigInteger& b2) {return b1.digits == b2.digits;}
bool operator !=(const BigInteger& b1, const BigInteger& b2) {return !(b1 == b2);}

BigInteger operator +(const BigInteger& b1, const BigInteger& b2) {
  BigInteger b3; int carry = 0;
  for (int k = 0; carry > 0 || k < (int)b1.digits.size() || k < (int)b2.digits.size(); ++k) {
    const int value = b1.get(k) + b2.get(k) + carry;
    b3.digits.push_back(value % BigInteger::BASE);
    carry = value / BigInteger::BASE;
  }
  return b3;
}

BigInteger& operator +=(BigInteger& b1, const BigInteger& b2) {
  int carry = 0;
  for (int k = 0; carry > 0 || k < (int)b2.digits.size(); ++k) {
    const int value = b1.get(k) + b2.get(k) + carry;
    b1.set(k, value % BigInteger::BASE);
    carry = value / BigInteger::BASE;
  }
  return b1;
}

BigInteger operator -(const BigInteger& b1, const BigInteger& b2) {
  BigInteger b3; int borrow = 0;
  for (int k = 0; k < (int)b1.digits.size() || k < (int)b2.digits.size(); ++k) {
    const int value = BigInteger::BASE + b1.get(k) - b2.get(k) - borrow;
    b3.digits.push_back(value % BigInteger::BASE);
    borrow = 1 - value / BigInteger::BASE;
  }
  return b3.normalize(), b3;
}

BigInteger& operator -=(BigInteger& b1, const BigInteger& b2) {
  int borrow = 0;
  for (int k = 0; k < (int)b1.digits.size() || k < (int)b2.digits.size(); ++k) {
    const int value = BigInteger::BASE + b1.get(k) - b2.get(k) - borrow;
    b1.set(k, value % BigInteger::BASE);
    borrow = 1 - value / BigInteger::BASE;
  }
  return b1.normalize(), b1;
}

BigInteger operator *(const BigInteger& b1, int X) {
  BigInteger b2; int carry = 0;
  for (int k = 0; k < (int)b1.digits.size(); ++k) {
    const int value = b1.digits[k] * X + carry;
    b2.digits.push_back(value % BigInteger::BASE);
    carry = value / BigInteger::BASE;
  }
  if (carry > 0) b2.digits.push_back(carry);
  return b2.normalize(), b2;
}

BigInteger& operator *=(BigInteger& b1, int X) {
  int carry = 0;
  for (int k = 0; k < (int)b1.digits.size(); ++k) {
    const int value = b1.digits[k] * X + carry;
    b1.digits[k] = value % BigInteger::BASE;
    carry = value / BigInteger::BASE;
  }
  if (carry > 0) b1.digits.push_back(carry);
  return b1.normalize(), b1;
}

BigInteger operator *(const BigInteger& b1, const BigInteger& b2) {
  BigInteger b3;
  for (int k = 0; k < (int)b2.digits.size(); ++k)
    b3 += (b1 * b2.digits[k]) << k;
  return b3;
}

BigInteger operator *=(BigInteger& b1, const BigInteger& b2) {
  BigInteger b3;
  for (int k = 0; k < (int)b2.digits.size(); ++k)
    b3 += (b1 * b2.digits[k]) << k;
  return b1 = b3;
}

BigInteger operator /(const BigInteger& b1, int X) {
  assert(X > 0);
  BigInteger b2; b2.digits.resize((int)b1.digits.size());
  for (int k = b1.digits.size() - 1, carry = 0; k >= 0; --k) {
    carry = carry * BigInteger::BASE + b1.digits[k];
    b2.digits[k] = carry / X;
    carry %= X;
  }
  return b2.normalize(), b2;
}

BigInteger& operator /=(BigInteger& b1, int X) {
  assert(X > 0);
  for (int k = (int)b1.digits.size() - 1, carry = 0; k >= 0; --k) {
    carry = carry * BigInteger::BASE + b1.digits[k];
    b1.digits[k] = carry / X;
    carry %= X;
  }
  return b1.normalize(), b1;
}

int operator %(const BigInteger& b1, int X) {
  assert(X > 0);
  int carry = 0;
  for (int k = (int)b1.digits.size() - 1; k >= 0; --k)
    carry = (carry * BigInteger::BASE + b1.digits[k]) % X;
  return carry;
}

BigInteger& operator %=(BigInteger& b1, int X) {
  assert(X > 0);
  int carry = 0;
  for (int k = (int)b1.digits.size() - 1; k >= 0; --k)
    carry = (carry * BigInteger::BASE + b1.digits[k]) % X;
  b1.digits.resize(1); b1.digits[0] = carry;
  return b1.normalize(), b1;
}

BigInteger operator /(const BigInteger& b1, const BigInteger& b2) {
  assert(b2.digits.size() > 0);
  BigInteger b3, carry; b3.digits.resize((int)b1.digits.size());
  for (int k = (int)b1.digits.size() - 1; k >= 0; --k) {
    carry <<= 1; carry.set(0, b1.digits[k]);
    int lower = 0, upper = BigInteger::BASE;
    while (lower < upper) {
      const int middle = (lower + upper) / 2;
      if (b2 * middle <= carry) lower = middle + 1;
      else upper = middle;
    }
    b3.digits[k] = lower - 1;
    carry -= b2 * b3.digits[k];
  }
  return b3.normalize(), b3;
}

BigInteger operator /=(BigInteger& b1, const BigInteger& b2) {
  assert((int)b2.digits.size() > 0); BigInteger carry;
  for (int k = (int)b1.digits.size() - 1; k >= 0; --k) {
    carry <<= 1; carry.set(0, b1.digits[k]);
    int lower = 0, upper = BigInteger::BASE;
    while (lower < upper) {
      const int middle = (lower + upper) / 2;
      if (b2 * middle <= carry) lower = middle + 1;
      else upper = middle;
    }
    b1.digits[k] = lower - 1;
    carry -= b2 * b1.digits[k];
  }
  return b1.normalize(), b1;
}

BigInteger operator %(const BigInteger& b1, const BigInteger& b2) {
  assert(b2.digits.size() > 0); BigInteger carry;
  for (int k = (int)b1.digits.size() - 1; k >= 0; --k) {
    carry <<= 1; carry.set(0, b1.digits[k]);
    int lower = 0, upper = BigInteger::BASE;
    while (lower < upper) {
      const int middle = (lower + upper) / 2;
      if (b2 * middle <= carry) lower = middle + 1;
      else upper = middle;
    }
    carry -= b2 * (lower - 1);
  }
  return carry;
}

BigInteger operator %=(BigInteger& b1, const BigInteger& b2) {
  assert(b2.digits.size() > 0);
  BigInteger carry;
  for (int k = (int)b1.digits.size() - 1; k >= 0; --k) {
    carry <<= 1; carry.set(0, b1.digits[k]);
    int lower = 0, upper = BigInteger::BASE;
    while (lower < upper) {
      const int middle = (lower + upper) / 2;
      if (b2 * middle <= carry) lower = middle + 1;
      else upper = middle;
    }
    carry -= b2 * (lower - 1);
  }
  return b1 = carry, b1;
}
//////////////////////////////////////////////////////////////////////////////////////////
BigInteger Power(const BigInteger& b, int p) {
  if (p == 0) return BigInteger(1);
  if (p == 1) return b;
  BigInteger x = Power(b, p / 2); x *= x;
  if (p % 2 != 0) x *= b; return x;
}

int GetNthRootDigit(const BigInteger& X, BigInteger& Y, int N) {
  int lower = 0, upper = BigInteger::BASE;
  while (lower < upper) {
    const int middle = (lower + upper) / 2;
    Y.set(0, middle);
    if (Power(Y, N) <= X) lower = middle + 1;
    else upper = middle;
  }
  return lower - 1;
}

BigInteger NthRoot(const BigInteger& b1, int N) {
  BigInteger X(0), Y(0), R(0);
  for (int B = (b1.digits.size() + N - 1) / N - 1; B >= 0; --B) {
    X <<= N, Y <<= 1;
    for (int k = 0; k < N; ++k)
      X.set(k, b1.get(B * N + k));
    X.normalize();
    Y.set(0, GetNthRootDigit(X, Y, N));
    R = X - Power(Y, N);
  }
  return Y;
}
//////////////////////////////////////////////////////////////////////////////////////////

pair<BigInteger, BigInteger> Eval(i64 n) {
  if (n == 1) return make_pair(BigInteger(3), BigInteger(1));
  if (n % 2 == 0) {
    const pair<BigInteger, BigInteger> res = Eval(n / 2);
    const BigInteger a = res.first * res.first + 5 * res.second * res.second;
    const BigInteger b = res.first * res.second * 2;
    return make_pair(a, b);
  } else {
    const pair<BigInteger, BigInteger> res = Eval(n - 1);
    const BigInteger a = res.first * 3 + res.second * 5;
    const BigInteger b = res.first + res.second * 3;
    return make_pair(a, b);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    i64 n; scanf("%lld", &n);
    const pair<BigInteger, BigInteger> res = Eval(n);
    const int a = res.first % 1000;
    const int b = NthRoot(res.second * res.second * 5, 2) % 1000;
    const int c = (a + b) % 1000;
    printf("Case #%d: %03d\n", Ti, c);
  }
  return 0;
}
