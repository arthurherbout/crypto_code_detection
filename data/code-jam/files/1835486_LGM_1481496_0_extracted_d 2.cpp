#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>
#include <cmath>
#include <queue>
#include <stack>
#include <set>
#include <utility>
#include <vector>
#include <bitset>
#include <deque>
#include <iomanip>
#include <complex>
#include <fstream>
#include <sstream>
#include <map>
//#include <climits>
//#include <list>

using namespace std;

#if ( _WIN32 || __WIN32__ )
    #define LLD "%I64d"
#else
    #define LLD "%lld"
#endif

#define show(x) cerr<<((#x))<<" = "<<((x))<<" "<<endl
#define bit(a,b) (((a)>>(b))&1)
#define gcd __gcd
#define endl '\n'
#define bcnt(x) ((__builtin_popcount(x)))
#define sz(x) ((int((x).size())))
#define sqr(x) ((((x))*((x))))
#define fx(x) fixed<<setprecision(x)

template<class T> inline void smn(T &a,const T &b){if(b<a) a=b;}
template<class T> inline void smx(T &a,const T &b){if(b>a) a=b;}
template<class T> inline T rev(const T & a){T _=a; reverse(_.begin(),_.end()); return _;}

typedef long long ll;
typedef long double ld;
typedef pair<int,int> pie;

const double eps=1e-9;
const ld leps=1e-14;

#define rep(i, n) for (int i = 0, _n = (int)(n); i < _n; i++)

int k;
string s;
string t = "48cd3f9h1jklmn0pqr57uvwxyz";
bool a[50][50];
int in[50], out[50];

int main()
{
	ios_base::sync_with_stdio(false);
	int ttt, ttc = 1;
	for (cin >> ttt; ttc <= ttt; ttc++)
	{
		memset (a, 0, sizeof a);
		memset (in, 0, sizeof in);
		memset (out, 0, sizeof out);
		cin >> k;
		cin >> s;
		int len = sz(s);
//		cerr << " &&&\n";
		rep (i, len-1)
		{
			int c1 = s[i] - 'a', c2 = s[i+1] - 'a';
			a[c1][c2] = true;
			if (isdigit (t[c1]))
				a[t[c1]-'0'+26][c2] = 1;
			if (isdigit (t[c2]))
				a[c1][t[c2]-'0'+26] = 1;
			if (isdigit (t[c1]) && isdigit (t[c2]))
				a[t[c1]-'0'+26][t[c2]-'0'+26] = 1;
		}
//		cerr << " &&&\n";
		rep (i, 36)
			rep (j, 36)
				if (a[i][j])
				{
					out[i]++, in[j]++;
/*					char c1, c2;
					if (i < 26)	c1 = i + 'a';
					else		c1 = i - 26 + '0';
					if (j < 26)	c2 = j + 'a';
					else		c2 = j - 26 + '0';
					cerr << i << ' ' << j << ' ' << c1 << ' ' << c2 << endl;
*/				}
		int r = 0, p = 0;
		rep (i, 36)
		{
//			if (out[i] || in[i]) cerr << " ** " << out[i] << ' ' << in[i] << endl;
			p += max (0, out[i] - in[i]);
			r += out[i];
		}
//		cerr << r << ' ' << p << endl;
		cout <<"Case #"<<ttc<<": "<< r + max (0, p-1) + 1 << endl;
	}

	return 0;
}
