// cheburashka, bear-mouse, team template

#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <stack>
#include <cstring>
#include <cmath>
#include <queue>
#include <set>
using namespace std;

typedef long long ll;
typedef vector < string > vs;
typedef vector <int> vi;
#define FOREACH(it,c) for(typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
#define deb(x) cout << #x << ": " << x << endl;
#define debv(x) for(int i = 0; i < (x).size(); i++) cout << x[i] << ' '; cout << endl;
#define pb(x) push_back(x)

//string split given string a and delimiters

inline double sq(double xa)
{
	return xa*xa;
}
inline double dist(double xa, double xb, double ya, double yb)
{
	return sqrt( sq(xa-xb) + sq(ya-yb) );
}
int main()
{
	int T;
	cin >> T;
	for(int tcase = 1; tcase <= T; tcase++)
	{
		int N,M;
		cin >> N >> M;
		double XA,YA,XB,YB;
		cin >> XA >> YA;
		cin >> XB >> YB;
		double P[M][2];
		for(int i = 0; i < M; i++)
		{
			cin >> P[i][0] >> P[i][1];
		}
		double ret[M],RA,RB;
		double D = dist(XA,XB,YA,YB);
		for(int i = 0; i < M; i++)
		{
			RA = dist(XA,P[i][0],YA,P[i][1]);
			RB = dist(XB,P[i][0],YB,P[i][1]);
			
			double ang1 = 2.0*acos( (RB*RB+D*D-RA*RA) / (2.0*RB*D) );
			double ang2 = 2.0*acos( (RA*RA+D*D-RB*RB) / (2.0*RA*D) );
			double A = .5*(ang1*RB*RB - RB*RB*sin(ang1));
			double B = .5*(ang2*RA*RA - RA*RA*sin(ang2));
			ret[i] = A + B;
		
		}
		printf("Case #%d:",tcase);
		for(int i = 0; i < M; i++)
		{
			printf(" %.7lf",ret[i]);
		}
		printf("\n");
	
	}
	return 0;
}
