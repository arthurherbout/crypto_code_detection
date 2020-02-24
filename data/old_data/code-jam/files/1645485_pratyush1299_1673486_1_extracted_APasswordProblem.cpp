/*
ID: vermapr1
PROG: <prog_name>
LANG: C++
*/

// #define TRUE FALSE haha now debug this
using namespace std;
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <cfloat>
#include <stack>
#include <map>
#include <set>
//BEGIN_TEMPLATE_BY_PRATYUSH_VERMA

//CONSTANT
#define INF (1<<31)-1
#define PI 3.1428571428571428571428571428571

//FUNC
#define MAX(i,j) (i)>(j)?(i):(j)
#define MIN(i,j) (i)<(j)?(i):(j)
#define REP(i,a) for((i)=0;(i)<(a);(i)++)
#define REP_(i,a) for((i)=0;(i)<=(a);(i)++)
#define FOR(i,a,b) for((i)=(a);(i)<(b);(i)++)
#define FOR_(i,a,b) for((i)=(a);(i)<=(b);(i)++)
#define VAR(V,init) __typeof(init) V=(init)
#define FOREACH(I,C) for(VAR(I,(C).begin());I!=(C).end();I++)
#define ALL(x) (x).begin(),(x).end()
#define SIZE(x) ((int)(x.size()))
#define LENGTH(x) ((int)(x.length()))
#define SZ(x) sizeof(x)
#define MEM(m,i) memset((m),(i),SZ(m))
#define PB(x,y) (x).push_back(y)
#define MP(x,y) make_pair(x,y)
#define V(x) vector < x >

//BITWISE
#define LASTSETBIT(x) x & ~(x-1)
#define TRAILZERO(x) __builtin_ctz(x)
#define LEADZERO(x) __builtin_clz(x)
#define COUNTSETBIT(x) __builtin_popcount(x)
#define GCD(x,y) __gcd(x,y)

typedef pair<int,int>   PII;
typedef pair<char,int>  PCI;
typedef pair<int,PII>   TRI;
typedef V( int )        VI;
typedef V( PII )        VII;
typedef V( TRI )        VTRI;
typedef V( string )     VS;
typedef long long       LL;
typedef long double     LD;

template<class T> void print(const vector<T> &v){ostringstream os; for(int i=0; i<v.size(); ++i){if(i)os<<' ';os<<v[i];} cout<<os.str()<<endl;}
template<class T, class U> T cast (U x) { ostringstream os; os<<x; T res; istringstream is(os.str()); is>>res; return res; }
template<class T> vector<T> split(string s, string x=" ") {vector<T> res; for(int i=0;i<s.size();i++){string a; while(i<(int)s.size()&&x.find(s[i])==string::npos)a+=s[i++]; if(!a.empty())res.push_back(cast<T>(a));} return res;}

inline void input( int &n ) { n=0; int ch=getchar(); while( ch < '0' || ch > '9' ) ch=getchar(); while( ch >= '0' && ch <= '9' ) n = (n<<3)+(n<<1) + ch-'0', ch=getchar(); }

//END_TEMPLATE_BY_PRATYUSH_VERMA

int main()
{
	int test;
	cin>>test;
	for(int case_no=1;case_no<=test;case_no++)
	{
		int A,B,i,back;
		cin>>A>>B;
		double prob[A];

		double all_corr = 1;
		REP(i,A)
		{
			double temp;
			cin>>temp;
			prob[i]=all_corr*(1.0-temp);
			all_corr *= temp;
		}
		double exp = DBL_MAX;
		REP(back,A)
		{
			double _exp = 0;
			//cout<<back<<" -> ";
			REP(i,A)
			{
				int type = B - A + 2*(back + 1) + 1;
				if((A-back-1) > i)
				{
					type += (B + 1);
				}
				//cout<<type<<" * "<<prob[i]<<endl;
				_exp += (type * prob[i]);
			}
			_exp += (all_corr * (B - A + 2*(back + 1) + 1));
			//cout<<"ALL CORR "<<(B - A + 2*(back + 1) + 1)<<" * "<<all_corr<<endl;
			exp = min(exp,_exp);
			//cout<<endl<<back<<" ---> "<<_exp<<" "<<exp<<"\n";
		}
		exp = min((all_corr * (B - A + 1)) + ((1-all_corr)* (B-A+1 +B+1)),exp);
		//cout<<"****** "<<(all_corr * (B - A + 1)) + ((1-all_corr)* (B-A+1 +B+1))<<endl;
	//	cout<<exp<<endl;
		exp = min(B + 2.0 , exp);
//		cout<<exp<<endl;
		printf("Case #%d: %.6lf\n",case_no,exp);
	}
//	system("pause");
	return 0;
}
