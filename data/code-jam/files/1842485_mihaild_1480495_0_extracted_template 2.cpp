/*
 * Template for code jam - different includes and templates
 * Real task code is in the end
 * Mikhail Dektyarow <mihail.dektyarow@gmail.com>, 2009
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <sstream>
#include <numeric>
#include <stack>
#include <bitset>
#include <iostream>
#include <string>
using namespace std;

/*
 * DEFINES
 */
#define FOR(i, a, b) for (int i(a), _b(b); i <= _b; ++i)
#define FORD(i, a, b) for (int i(a), _b(b); i >= _b; --i)
#define REP(i, n) for (int i(0), _n(n); i < _n; ++i)
#define REPD(i, n) for (int i((n)-1); i >= 0; --i)
#define ALL(c) (c).begin(), (c).end()
#define SIZE(X) ((int)(X.size()))
#define LENGTH(X) ((int)(X.length()))

/*
 * Types
 */
typedef pair<int,int> ipair;
typedef long long int int64;
typedef unsigned long long uint64;

/*
 * Different useful templates
 */
template<typename T> void remin(T& a, const T& b) { if (b < a) a = b; }
template<typename T> void remax(T& a, const T& b) { if (b > a) a = b; }
template<class T1, class T2>inline istream& operator>> (istream& s, pair<T1, T2> &p) {	return s >> p.first >> p.second;}
template<class T1, class T2>inline ostream& operator<< (ostream& s, const pair<T1, T2>p) {	return s << "(" << p.first << " " << p.second << ")";}
template<class T1>inline ostream& operator<< (ostream& s, const vector<T1> container) {
	for (typename vector<T1>::const_iterator i = container.begin(); i != container.end(); i++) s << *i << " ";
	return s;
}
template<class T1>inline istream& operator>> (istream&s, vector<T1> &container) {
	for (typename vector<T1>::iterator i = container.begin(); i != container.end(); i++) s >> *i;
	return s;
}
/*
 * Euclide's algorithm
 */
template<class T> inline T euclide(T a,T b,T &x,T &y)
  {if(a<0){T d=euclide(-a,b,x,y);x=-x;return d;}
   if(b<0){T d=euclide(a,-b,x,y);y=-y;return d;}
   if(b==0){x=1;y=0;return a;}else{T d=euclide(b,a%b,x,y);T t=x;x=y;y=t-(a/b)*y;return d;}}
/*
 * Factorizing a number
 */
template<class T> inline vector<pair<T,int> > factorize(T n)
{vector<pair<T,int> > R;for (T i=2;n>1;){if (n%i==0){int C=0;for (;n%i==0;C++,n/=i);R.push_back(make_pair(i,C));}i++;if (i>n/i) i=n;}if (n>1) R.push_back(make_pair(n,1));return R;}
template<class T> inline bool isPrimeNumber(T n)
  {if(n<=1)return false;for (T i=2;i*i<=n;i++) if (n%i==0) return false;return true;}
//Searching prime numbers
//Using Sieve of Atkin (http://en.wikipedia.org/wiki/Sieve_of_Atkin)
vector<int> gen_primes(int limit) {
	int sqr_lim;
	int x2, y2;
	int n;
	vector<bool> is_prime(limit+1, false);
	sqr_lim = (int)sqrt((long double)limit);
	is_prime[2] = true;
	is_prime[3] = true;
	x2 = 0;
	for (int i = 1; i <= sqr_lim; i++) {
		x2 += 2 * i - 1;
		y2 = 0;
		for (int j = 1; j <= sqr_lim; j++) {
			y2 += 2 * j - 1;
			n = 4 * x2 + y2;
			if ((n <= limit) && (n % 12 == 1 || n % 12 == 5))
				is_prime[n] = !is_prime[n];
			n -= x2;
			if ((n <= limit) && (n % 12 == 7))
				is_prime[n] = !is_prime[n];
			n -= 2 * y2;
			if ((i > j) && (n <= limit) && (n % 12 == 11))
				is_prime[n] = !is_prime[n];
		}
	}
	for (int i = 5; i <= sqr_lim; i++) {
		if (is_prime[i]) {
			n = i * i;
			for (int j = n; j <= limit; j += n) {
				is_prime[j] = false;
			}
		}
	}
	int primes_count = 0;
	for (int i = 2; i < limit; i++) primes_count += is_prime[i];
	vector<int> primes;
	primes.reserve(primes_count);
	for (int i=2; i < limit; i++) if (is_prime[i]) primes.push_back(i);
	return primes;
}
//Translating string to different types
template<class T> string toString(T n){ostringstream ost;ost<<n;ost.flush();return ost.str();}//NOTES:toString(
int toInt(string s){int r=0;istringstream sin(s);sin>>r;return r;}//NOTES:toInt(
int64 toInt64(string s){int64 r=0;istringstream sin(s);sin>>r;return r;}//NOTES:toInt64(
double toDouble(string s){double r=0;istringstream sin(s);sin>>r;return r;}//NOTES:toDouble(

/*
 * Real code
 */

int main(void) {
	int __number_of_cases;
	cin >> __number_of_cases;
	REP(__number_of_case, __number_of_cases) {
		cout << "Case #" << __number_of_case+1 << ":\n";
        int R, C;
        cin >> R >> C;
        vector<vector<char> > RC1(R, vector<char>(C));
        cin >> RC1;
        vector<vector<char> > RC(C, vector<char>(C));
        REP(j, C) {
            REP(i, R) {
                RC[j][i] = RC1[i][j];
            }
            reverse(RC[j].begin(), RC[j].end());
        }
        swap(R, C);
        //reverse(RC.begin(), RC.end());
        vector<ipair> caves(10, ipair(-1, -1));
        REP(i, R) {
            REP(j, C) {
                if (RC[i][j] >= '0' && RC[i][j] <= '9') {
                    caves[RC[i][j] - '0'] = ipair(i, j);
                }
            }
        }
        //REP(i, R) {
            //REP(j, C) {
                //cout << '|' << RC[i][j] << '|';
            //}
            //cout << '\n';
        //}
        REP(c, 10) {
            if (caves[c].first == -1) {
                break;
            }
            cout << c << ": ";
            vector<vector<bool> > possible(R, vector<bool>(C, false));
            possible[caves[c].first][caves[c].second] = true;
            queue<ipair> targets;
            targets.push(caves[c]);
            while (!targets.empty()) {
                ipair target(targets.front());
                //cout << target << ' ' << RC[target.first][target.second] << '\n';
                targets.pop();
                if (target.first >= 1 && !possible[target.first-1][target.second] && RC[target.first-1][target.second] != '#') {
                    targets.push(ipair(target.first-1, target.second));
                    possible[target.first-1][target.second] = true;
                }
                if (target.first < R-1 && !possible[target.first+1][target.second] && RC[target.first+1][target.second] != '#') {
                    targets.push(ipair(target.first+1, target.second));
                    possible[target.first+1][target.second] = true;
                }
                if (target.second < C-1 && !possible[target.first][target.second+1] && RC[target.first][target.second+1] != '#') {
                    targets.push(ipair(target.first, target.second+1));
                    possible[target.first][target.second+1] = true;
                }
            }
            //cout << possible << '\n';
            int ok(0);
            REP(i, R) {
                REP(j, C) {
                    if (possible[i][j]) {
                        ++ok;
                    }
                }
            }
            cout << ok << " ";
            bool lucky(true);
            REP(i, R) {
                REP(j, C) {
                    if (!possible[i][j]) {
                        continue;
                    }
                    //cout << "\n\n";
                    vector<vector<vector<vector<bool> > > >  mp(R, vector<vector<vector<bool> > > (C, vector<vector<bool> > (R, vector<bool> (C, false))));
                    mp[caves[c].first][caves[c].second][i][j] = true;
                    //cout << mp[caves[c].first][caves[c].second][caves[c].first][caves[c].second] << '\n';
                    queue<pair<ipair, ipair> > targets;
                    targets.push(pair<ipair, ipair>(caves[c], ipair(i, j)));
                    while (!targets.empty()) {
                        pair<ipair, ipair> next(targets.front());
                        if (c == 3) {
                            //cout << next << '\n';
                        }
                        if (next.first.first > 0 && RC[next.first.first-1][next.first.second] != '#') {
                            next.first.first -= 1;
                        }
                        if (next.second.first > 0 && RC[next.second.first-1][next.second.second] != '#') {
                            next.second.first -= 1;
                        }
                        if (!mp[next.first.first][next.first.second][next.second.first][next.second.second]) {
                            targets.push(next);
                            mp[next.first.first][next.first.second][next.second.first][next.second.second] = true;
                        }
                        next = targets.front();
                        if (next.first.first < R-1 && RC[next.first.first+1][next.first.second] != '#') {
                            next.first.first += 1;
                        }
                        if (next.second.first < R-1 && RC[next.second.first+1][next.second.second] != '#') {
                            next.second.first += 1;
                        }
                        if (!mp[next.first.first][next.first.second][next.second.first][next.second.second]) {
                            targets.push(next);
                            mp[next.first.first][next.first.second][next.second.first][next.second.second] = true;
                        }
                        next = targets.front();
                        if (next.first.second > 0 && RC[next.first.first][next.first.second-1] != '#') {
                            next.first.second -= 1;
                        }
                        if (next.second.second > 0 && RC[next.second.first][next.second.second-1] != '#') {
                            next.second.second -= 1;
                        }
                        if (!mp[next.first.first][next.first.second][next.second.first][next.second.second]) {
                            targets.push(next);
                            mp[next.first.first][next.first.second][next.second.first][next.second.second] = true;
                        }
                        next = targets.front();
                        targets.pop();
                    }
                    //cout << mp[caves[c].first][caves[c].second][caves[c].first][caves[c].second] << '\n';
                    if (!mp[caves[c].first][caves[c].second][caves[c].first][caves[c].second]) {
                        lucky = false;
                        break;
                    }
                }
                if (!lucky) {
                    break;
                }
            }
            cout << (lucky ? "Lucky" : "Unlucky") << '\n';
        }
	}
}
