#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
using namespace std; 

#define DEBUG(x) fout << '>' << #x << ':' << x << endl;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR2(i, a, b) for (int i = (a); i > (b); --i)
inline bool EQ(double a, double b) { return fabs(a-b) < 1e-9; }
const int INF = 1 << 29;

inline int two(int n) { return 1 << n; }
inline int test(int n, int b) { return n & two(b); }
inline void set_bit(int & n, int b) { n |= two(b); }
inline void unset_bit(int & n, int b) { n &= ~two(b); }
#ifdef WIN32
typedef __int64 ll;
#else
typedef long long ll;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

ofstream fout("output.out");
ifstream fin("input.in");

const int MAX = 1000047;
bool no_prime[1000047];
vector<ll> primes;

void init()
{
	for (ll i = 2; i < MAX; ++i)
		if (!no_prime[i])
		{
			primes.push_back(i);
			for (ll j = i*i; j < MAX; j+=i)
				no_prime[j] = true;
		}
}

vector<ll> factorize(ll num)
{
	vector<ll> res;
	for (int i = 0; num > 1 && primes[i] * primes[i] <= num; ++i)
		if (num % primes[i] == 0)
		{
			res.push_back(primes[i]);
			while (num % primes[i] == 0) num /= primes[i];
		}
	if (num > 1) res.push_back(num);
	return res;
}

bool used[1000047];

int main()
{
	init();

	int T;
	fin >> T;

	FOR(step, 0, T)
	{
		ll A, B, P;
		fin >> A >> B >> P;

		ll len = B-A+1;
		FOR(i, 0, len) used[i] = false;

		int res = 0;
		FOR(i, 0, len)
			if (!used[i])
			{
				used[i] = true;

				++res;
				vector<ll> fact = factorize((ll)i + A);

				set<ll> u;
				queue<pair<ll, ll> > manage;

				FOR(j, 0, fact.size()) 
					if (fact[j] >= P)
					{ 
						u.insert(fact[j]); 
						manage.push(make_pair((ll)i+A, fact[j]));
					}

				while (!manage.empty())
				{
					pair<ll, ll> temp = manage.front();
					manage.pop();

					for (ll j = temp.first; j <= B; j += temp.second)
						if (!used[j-A])
						{
							used[j-A] = true;
							fact = factorize(j);

							FOR(k, 0, fact.size())
								if (fact[k] >= P && u.find(fact[k]) == u.end())
								{
									u.insert(fact[k]);
									manage.push(make_pair(A + fact[k] - A%fact[k], fact[k]));
								}
						}
				}
			}

		fout << "Case #" << step+1 << ": " << res << endl;
	}

	return 0;
}
