#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <cstring>
#include <cassert>
#include <bitset>
#include <sstream>
#include <queue>

#define pb push_back
#define mp make_pair
#define fs first
#define sc second
#define sz(a) ((int) (a).size())
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

typedef long long int64;
typedef long double ldb;

const long double eps = 1e-9;
const int inf = (1 << 30) - 1;
const long long inf64 = ((long long)1 << 62) - 1;
const long double pi = acos(-1);

template <class T> T sqr (T x) {return x * x;}
template <class T> T abs (T x) {return x < 0 ? -x : x;}

const int MOD = 10000;
const int MAXN = 2000;

struct TBig {
	int dig[MAXN];
	int num = 0;

	TBig(int x) {
		num = 0;
		while (x > 0) {
			dig[num] = x % MOD;
			++num;
			x /= MOD;
		}
	}

	TBig() = default;
};

string ToString(const TBig& a, int sign = 1) {
	bool started = false;
	string res = "";
	if (sign == -1) {
		res = "-";
	}

	for (int i = a.num - 1; i >= 0; --i) {
		int cur = a.dig[i];
		for (int mod = MOD / 10; mod >= 1; mod /= 10) {
			if (started) {
				res.pb('0' + (cur / mod));
			} else {
				if (cur >= mod) {
					started = true;
					res.pb('0' + (cur / mod));
				}
			}
			cur %= mod;
		}
	}

	if (!started) {
		res = "0";
	}

	return res;
}

TBig Sum(const TBig& a, const TBig& b) {
	TBig c;
	c.num = max(a.num, b.num);

	int cur = 0;
	for (int i = 0; i < c.num; ++i) {
		if (i < a.num) {
			cur += a.dig[i];
		}
		if (i < b.num) {
			cur += b.dig[i];
		}
		c.dig[i] = cur % MOD;
		cur /= MOD;
	}

	while (cur > 0) {
		c.dig[c.num] = cur % MOD;
		++c.num;
		cur /= MOD;
	}

	while (c.num > 0 && c.dig[c.num - 1] == 0) {
		--c.num;
	}

	return c;
}

TBig Mult(const TBig& a, int x) {
	TBig b;
	b.num = a.num;

	long long cur = 0;
	for (int i = 0; i < b.num; ++i) {
		cur += a.dig[i] * (long long) x;
		b.dig[i] = cur % MOD;
		cur /= MOD;
	}

	while (cur > 0) {
		b.dig[b.num] = cur % MOD;
		++b.num;
		cur /= MOD; 
	}

	while (b.num > 0 && b.dig[b.num - 1] == 0) {
		--b.num;
	}

    return b;
}

int Mod(const TBig& a, int x) {
	int cur = 0;
	for (int i = a.num - 1; i >= 0; --i) {
		cur = cur * MOD + a.dig[i];
		cur = cur % x;
	}
    return cur;
}

TBig Divide(const TBig& a, int x) {
	TBig b;
	b.num = a.num;

	int cur = 0;
	for (int i = b.num - 1; i >= 0; --i) {
		cur = cur * MOD + a.dig[i];
		b.dig[i] = cur / x;
		cur = cur % x;
	}

	while (b.num > 0 && b.dig[b.num - 1] == 0) {
		--b.num;
	}

	return b;
}

bool Less(const TBig& a, const TBig& b) {
	if (a.num != b.num) {
		return a.num < b.num;
	}

    int ind = a.num - 1;
	while (ind >= 0 && a.dig[ind] == b.dig[ind]) {
		--ind;
	}

	return (ind >= 0 && a.dig[ind] < b.dig[ind]);
}

TBig Diff(const TBig& a, const TBig& b) {
	TBig c;
	c.num = a.num;

	int cur = 0;
	for (int i = 0; i < c.num; ++i) {
		if (i < a.num) {
			cur += a.dig[i];
		}
		if (i < b.num) {
			cur -= b.dig[i];
		}

		if (cur >= 0) {
			c.dig[i] = cur;
			cur = 0;
		} else {
			c.dig[i] = MOD + cur;
			cur = -1;
		}
	}

	while (c.num > 0 && c.dig[c.num - 1] == 0) {
		--c.num;
	}

	return c;
}

const int MAXP = 1000;

int main () {
	ios_base::sync_with_stdio(0);
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	vector<int> p;
	for (int i = 2; i < MAXP; ++i) {
		bool key = true;
		for (int j = 0; j < sz(p); ++j) {
			if (i % p[j] == 0) {
				key = false;
				break;
			}
		}
		if (key) {
			p.pb(i);
		}
	}

	int tc;
	cin >> tc;
	for (int ti = 0; ti < tc; ++ti) {
	    int n, init;
	    cin >> init >> n;

	    int a_plus = 0, a_minus = 0;
	    vector<int> mult;
	    vector<int> divide;
	    bool has_zero = false;
	    for (int i = 0; i < n; ++i) {
	    	char c;
	    	int x;
	    	cin >> c >> x;
	    	
	    	if (c == '+') {
	    		if (x > 0) {
                	a_plus += x;
               	} else {
            		a_minus -= x;
            	}
	    	} else if (c == '-') {
	    		if (x > 0) {
	    			a_minus += x;
	    		} else {
	    			a_plus -= x;
	    		}
	    	} else if (c == '*') {
	    		if (x != 0) {
	    			mult.pb(x);
	    		} else {
	    			has_zero = true;
	    		}
	    	} else {
	    		divide.pb(x);
	    	}
	    }

	    if (has_zero) {
	    	init = 0;
	    }

	    sort(mult.begin(), mult.end());
	    sort(divide.begin(), divide.end());

	    TBig m_mult(1);
	    int sign_mult = 1;
	    for (int i = 0; i < sz(mult); ++i) {
	    	m_mult = Mult(m_mult, abs(mult[i]));
	    	if (mult[i] < 0) {
	    		sign_mult *= -1;
	    	}
	    }
	    
	    TBig m_div(1);
	    TBig m_both = m_mult;
        int sign_div = 1;
        for (int i = 0; i < sz(divide); ++i) {
        	m_div = Mult(m_div, abs(divide[i]));
        	m_both = Mult(m_both, abs(divide[i]));
        	if (divide[i] < 0) {
        		sign_div *= -1;
        	}
        }

        pair<TBig, TBig> en;
        en.fs = 0;
        en.sc = 0;
        if ((sz(mult) > 0 && mult[0] < 0) || (sz(divide) > 0 && divide[0] < 0)) {
	        int max_neg = -(MOD + 1);
	        for (int i = 0; i < sz(mult); ++i) {
	        	if (mult[i] < 0) {
					max_neg = max(max_neg, mult[i]);
	           	}
	        }
	        for (int i = 0; i < sz(divide); ++i) {
	        	if (divide[i] < 0) {
	        		max_neg = max(max_neg, divide[i]);
	        	}
	        }

	        if (sign_mult == -1) {
	        	swap(a_plus, a_minus);
	        }

            en.fs = Mult(m_both, a_plus);
            en.fs = Sum(en.fs, Mult(Divide(m_both, -max_neg), a_minus));
        } else if (has_zero) {
	    	en.fs = Mult(m_both, a_plus);
	    } else {
	    	vector<pair<TBig, TBig>> to_try(4);
	    	to_try[0] = mp(Mult(m_both, a_plus), Mult(m_mult, a_minus));
	    	to_try[1] = mp(Mult(m_div, a_plus), a_minus);
	    	to_try[2] = mp(Mult(m_mult, a_plus), a_minus);
	    	to_try[3] = mp(Mult(m_both, a_plus), Mult(m_div, a_minus));
	    
	    	en = to_try[0];
	    	for (int i = 1; i < 4; ++i) {
	    		if (Less(Sum(en.fs, to_try[i].sc), Sum(to_try[i].fs, en.sc))) {
	    			en = to_try[i];
	    		}
	    	}
	    }

	    if (init * sign_mult * sign_div < 0) {
			en.sc = Sum(en.sc, Mult(m_mult, abs(init)));
        } else {
          	en.fs = Sum(en.fs, Mult(m_mult, abs(init)));
        }

        int sign_res = 1;
        if (Less(en.fs, en.sc)) {
       		sign_res = -1;
       		swap(en.fs, en.sc);
        }

        TBig en_res = Diff(en.fs, en.sc);
    	TBig den_res = 1;
    	for (int i = 0; i < sz(divide); ++i) {
    		int val = abs(divide[i]);
    		for (int j = 0; j < sz(p); ++j) {
    			while (val % p[j] == 0 && Mod(en_res, p[j]) == 0) {
    				val /= p[j];
    				en_res = Divide(en_res, p[j]);
    			}
    		}
    		den_res = Mult(den_res, val);
    	}

		cout << "Case #" << ti + 1 << ": " << ToString(en_res, sign_res) << " " << ToString(den_res) << endl;
		cerr << "Done " << ti + 1 << endl;
	}

	return 0;
}