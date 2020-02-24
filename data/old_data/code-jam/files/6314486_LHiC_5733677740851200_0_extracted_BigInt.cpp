#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define ratio ratio228

struct BigInt {
    const static ll BASE = 1000000;
    vector<ll> a;
    int sgn;
    BigInt() {
        a.clear();
        a.push_back(0);
        sgn = 0;
    };
    void opti() {
        while (a.size() >= 2 && a.back() == 0)
            a.pop_back();
        if (a.size() == 1 && a[0] == 0)
            sgn = 0;
    }
    BigInt(ll x) {
        if (x < 0)
            sgn = 1, x = -x;
        else
            sgn = 0;
        a.clear();
        while (x) {
            a.push_back(x % BASE);
            x /= BASE;
        }
        if (a.empty())
            a.push_back(0);
    }
    BigInt(int x) {
        if (x < 0)
            sgn = 1, x = -x;
        else
            sgn = 0;
        a.clear();
        while (x) {
            a.push_back(x % BASE);
            x /= BASE;
        }
        if (a.empty())
            a.push_back(0);
    }
    BigInt(const char *s) {
        sgn = 0;
        a.clear();
        if (s == 0 || s[0] == 0) {
            a.push_back(0);
            return;
        }
        int n = strlen(s);
        if (s[0] == '-') {
            s += 1;
            sgn = 1;
            --n;
        }
        ll now = 1;
        a.push_back(0);
        for (int i = n - 1; i >= 0; --i) {
            a.back() += now * (s[i] - '0');
            now *= 10;
            if (now == BASE) {
                a.push_back(0);
                now = 1;
            }
        }
        opti();
    }
    BigInt(const string &s): BigInt(s.c_str()) {
    }
    BigInt operator+(const BigInt &x) {
        BigInt ans = *this;
        ans.a.resize(max(ans.a.size(), x.a.size()) + 1);
        if (ans.sgn == x.sgn) {
            for (int i = 0; i < x.a.size(); ++i)
                ans.a[i] += x.a[i];
            ll p = 0;
            for (int i = 0; i < ans.a.size(); ++i) {
                ans.a[i] += p;
                p = ans.a[i] / BASE;
                ans.a[i] %= BASE;
            }
        }
        else {
            for (int i = 0; i < x.a.size(); ++i)
                ans.a[i] -= x.a[i];
            ll p = 0;
            for (int i = 0; i < ans.a.size(); ++i) {
                ans.a[i] -= p;
                if (ans.a[i] < 0)
                    ans.a[i] += BASE, p = 1;
                else 
                    p = 0;
            }
            if (p == 1) {
                ans.sgn ^= 1;
                ll p = 0;
                for (int i = 0; i < ans.a.size(); ++i) {
                    ans.a[i] = -ans.a[i];
                    ans.a[i] -= p;
                    if (ans.a[i] < 0)
                        ans.a[i] += BASE, p = 1;
                    else
                        p = 0;
                }
            }
        }
        ans.opti();
        return ans;
    }
    BigInt operator-(const BigInt &x) {
        BigInt ans = *this;
        ans.a.resize(max(ans.a.size(), x.a.size()) + 1);
        if (ans.sgn != x.sgn) {
            for (int i = 0; i < x.a.size(); ++i)
                ans.a[i] += x.a[i];
            ll p = 0;
            for (int i = 0; i < ans.a.size(); ++i) {
                ans.a[i] += p;
                p = ans.a[i] / BASE;
                ans.a[i] %= BASE;
            }
        }
        else {
            for (int i = 0; i < x.a.size(); ++i)
                ans.a[i] -= x.a[i];
            ll p = 0;
            for (int i = 0; i < ans.a.size(); ++i) {
                ans.a[i] -= p;
                if (ans.a[i] < 0)
                    ans.a[i] += BASE, p = 1;
                else 
                    p = 0;
            }
            if (p == 1) {
                ans.sgn ^= 1;
                ll p = 0;
                for (int i = 0; i < ans.a.size(); ++i) {
                    ans.a[i] = -ans.a[i];
                    ans.a[i] -= p;
                    if (ans.a[i] < 0)
                        ans.a[i] += BASE, p = 1;
                    else
                        p = 0;
                }
            }
        }
        ans.opti();
        return ans;
    }
    BigInt operator*(const BigInt &x) {
        BigInt ans;
        ans.a.resize(x.a.size() + a.size() + 1);
        for (int i = 0; i < a.size(); ++i)
            for (int j = 0; j < x.a.size(); ++j) {
                ans.a[i + j] += a[i] * x.a[j];
            }
        ll p = 0;
        for (int i = 0; i < ans.a.size(); ++i) {
            ans.a[i] += p;
            p = ans.a[i] / BASE;
            ans.a[i] %= BASE;
        }
        ans.sgn = x.sgn ^ sgn;
        ans.opti();
        return ans;
    }
    BigInt operator-() {
        BigInt ans = *this;
        ans.sgn ^= 1;
        ans.opti();
        return ans;
    }
    BigInt operator/(ll x) {
        BigInt ans = *this;
        if (x < 0)
            x = -x, ans.sgn ^= 1;
        ll p = 0;
        for (int i = ans.a.size() - 1; i >= 0; --i) {
            ans.a[i] += p;
            p = ans.a[i] % x;
            ans.a[i] /= x;
            p *= BASE;
        }
        if (ans.sgn && p)
            return ans - "1";
        return ans;
    }
    ll operator%(ll x) {
        ll p = 0;
        for (int i = a.size() - 1; i >= 0; --i)
            p = (p * BASE + a[i]) % x;
        if (sgn) {
            p = -p;
            if (p < 0)
                p += x;
        }
        return p;
    }
    string toString() {
        string s;
        for (int i = 0; i < a.size(); ++i) {
            ll now = 1;
            ll x = a[i];
            while (now != BASE) {
                s += (char)('0' + x % 10);
                x /= 10;
                now *= 10;
            }
        }
        while (s.size() >= 2 && s.back() == '0')
            s.pop_back();
        if (sgn)
            s += "-";
        reverse(s.begin(), s.end());
        return s;
    }
    BigInt operator+=(const BigInt &x) {
        *this = *this + x;
        return *this;
    }
    BigInt operator-=(const BigInt &x) {
        *this = *this - x;
        return *this;
    }
    BigInt operator*=(const BigInt &x) {
        *this = *this * x;
        return *this;
    }
    BigInt operator/=(ll x) {
        *this = *this / x;
        return *this;
    }
    BigInt operator%=(ll x) {
        ll y = *this % x;
        *this = BigInt(y);
        return *this;
    }
};

bool operator==(const BigInt &x, const BigInt &y) {
    if (y.sgn != x.sgn)
        return 0;
    if (y.a != x.a)
        return 0;
    return 1;
}

bool operator<(const BigInt &x, const BigInt &y) {
    if (x.sgn) {
        if (y.sgn) {
            if (x.a.size() > y.a.size())
                return 1;
            if (x.a.size() < y.a.size())
                return 0;
            for (int i = x.a.size() - 1; i >= 0; --i) {
                if (x.a[i] > y.a[i])
                    return 1;
                else if (x.a[i] < y.a[i])
                    return 0;
            }
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        if (y.sgn) {
            return 0;
        }
        else {
            if (x.a.size() > y.a.size())
                return 0;
            if (x.a.size() < y.a.size())
                return 1;
            for (int i = x.a.size() - 1; i >= 0; --i) {
                if (x.a[i] > y.a[i])
                    return 0;
                else if (x.a[i] < y.a[i])
                    return 1;
            }
            return 0;

        }
    }
}

bool operator>(const BigInt &x, const BigInt &y) {
    return y < x;
}

bool operator<=(const BigInt &x, const BigInt &y) {
    return !(x > y);
}

bool operator>=(const BigInt &x, const BigInt &y) {
    return !(x < y);
}

struct ratio {
	BigInt p, q;
	ratio() {
		p = 0;
		q = 1;
	}
	void chck() {
		if (p == 0)
			p = 0, q = 1;
	}
	void operator+=(ll x) {
		p = p + q * x;
		chck();
	}
	void chsgn() {
		p = -p;
	}
	void mp(ll x) {
		if (x != 0 && q % x == 0)
			q = q / x;
		else
			p = p * x;
		chck();
	}
	void dl(ll x) {
		if (x != 0 && p % x == 0)
			p = p / x;
		else
			q = q * x;
		chck();
	}
	bool operator<(ratio x) {
		BigInt a = p * x.q;
		BigInt b = x.p * q;
		return a < b;
	}
};


void print(ratio x) {
	cout << x.p.toString() << " " << x.q.toString() << "\n";
}

int s;

vector<pair<int, vector<ll> > > vv;

ratio get() {
	ratio ans;
	ans += s;
	for (int i = 0; i < vv.size(); ++i) {
		if (vv[i].first == 0) {
			ans += vv[i].second[0];
		}
		else if (vv[i].first == 1) {
			ans += -vv[i].second[0];
		}
		else if (vv[i].first == 2) {
			for (ll j: vv[i].second) {
				ans.mp(j);
			}
		}
		else if (vv[i].first == 3) {
			for (ll j: vv[i].second) {
				ans.dl(j);
			}
		}
		else if (vv[i].first == 4) {
			for (ll j: vv[i].second) {
				ans.mp(j);
			}
			ans.chsgn();
		}
		else {
			for (ll j: vv[i].second) {
				ans.dl(j);
			}
			ans.chsgn();
		}
	}
	return ans;
}


vector<ll> vmp;
vector<ll> vmn;
vector<ll> vdp;
vector<ll> vdn;

vector<ll> ve(ll x) {
	vector<ll> ans;
	ans.push_back(x);
	return ans;
}

vector<ll> ve1(vector<ll> x) {
	vector<ll> ans;
	ans.push_back(x[0]);
	return ans;
}

vector<ll> ve2(vector<ll> x) {
	vector<ll> ans;
	for (int i = 1; i < x.size(); ++i)
		ans.push_back(x[i]);
	return ans;
}

void solve() {
	int n;
	cin >> s >> n;
	ll sp = 0;
	ll sn = 0;
	vmp.clear();
	vdp.clear();
	vmn.clear();
	vdn.clear();
	vector<ll> v1;
	vector<ll> v2;
	for (int i = 0; i < n; ++i) {
		char c;
		ll x;
		scanf(" %c %lld", &c, &x);
		if (c == '+') {
			if (x > 0)
				sp += x;
			else
				sn -= x;
		}
		else if (c == '-') {
			if (x > 0)
				sn += x;
			else
				sp -= x;
		}
		else if (c == '*') {
			if (x > 0) {
				for (int i = 2; i * i <= x; ++i) {
					while (x % i == 0)
						vmp.push_back(i), x /= i;
				}
				if (x != 1)
					vmp.push_back(x);
			}
			else {
				vmn.push_back(-x);
			}
		}
		else {
			if (x > 0) {
				for (int i = 2; i * i <= x; ++i) {
					while (x % i == 0)
						vdp.push_back(i), x /= i;
				}
				if (x != 1)
					vdp.push_back(x);
			}
			else {
				vdn.push_back(-x);
			}
		}
	}
	sort(vdn.begin(), vdn.end());
	sort(vmn.begin(), vmn.end());
	vv.clear();
	vv.push_back(make_pair(0, ve(sp)));
	vv.push_back(make_pair(1, ve(sn)));
	vv.push_back(make_pair(2, vmp));
	vv.push_back(make_pair(3, vdp));
	if (vmn.size()) {
		vv.push_back(make_pair(4, ve1(vmn)));
		if (vmn.size() % 2 == 0)
			vv.push_back(make_pair(4, ve2(vmn)));
		else
			vv.push_back(make_pair(2, ve2(vmn)));
	}
	if (vdn.size()) {
		vv.push_back(make_pair(5, ve1(vdn)));
		if (vdn.size() % 2 == 0)
			vv.push_back(make_pair(5, ve2(vdn)));
		else
			vv.push_back(make_pair(3, ve2(vdn)));
	}
	sort(vv.begin(), vv.end());
	for (int i = 0; i < vv.size(); ++i) {
		if (vv[i].first <= 2)
			continue;
		vector<ll> vv1;
		for (int j: vv[i].second) {
			int x = j;
			for (int k = 2; k * k <= j; ++k) {
				while (x % k == 0)
					vv1.push_back(k), x /= k;
			}
			if (x != 1)
				vv1.push_back(x);
		}
		vv[i].second = vv1;
	}
	ratio ans = get();
	do {
		ratio now = get();
		if (ans < now)
			ans = now;
	} while (next_permutation(vv.begin(), vv.end()));
	print(ans);
}


int main() {
	int tt;
	scanf("%d", &tt);
	for (int i = 0; i < tt; ++i) {
		printf("Case #%d: ", i + 1);
		solve();
	}
}




