#if 1
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <sstream>
#include <stack>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <functional>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <cstdio>
using namespace std;

typedef long long LL;
typedef long double LD;
const LD eps = 1e-9;

typedef pair<int, int> pii;
#define mp make_pair
#define pb push_back
#define X first
#define Y second
#define iss istringstream
#define oss ostringstream
#define dbg(x) cerr << #x << " = " << x << endl;

const LD pi = 3.141592653589793;

LD dist(LD x1, LD y1, LD x2, LD y2)
{
	return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}

LD intersect(LD x1, LD y1, LD r1, LD x2, LD y2, LD r2)
{
	LD D, d, square;
	D = dist(x1, y1, x2, y2);

	x1 = y1 = 0.0;
	y1 = 0;
	x1 = D;
	d = D;
	if(r1 < r2) 
		swap(r1, r2);

	if(D > r1 + r2) 
		square = 0.0;
	else if(fabs(r1 - r2) >= D) 
		square  = min(r1, r2) * min(r1, r2) * pi;
	else 
	{
		LD l, h;
		bool fl;
		LD betta = acos( (r2*r2+d*d-r1*r1)/(2*r2*d) );
//		printf("%.10lf\n",d);
		if(betta < pi/2.0) fl = true;
		else fl = false;
		if(fl) 
		{
            l = (r2*r2 - r1*r1 + d*d) / (2.0 * d);
            h = sqrt(r2*r2 - l*l);
		} else 
		{
            l = (r1*r1 - r2*r2 - d*d) / (2.0 * d);
            h = sqrt(r2*r2 - l*l);
//            return -1;
		}
		LD s1;
		LD s2;
		if(fl) 
		{
		    s1 = h * (d - l);
		    s2 = h * l;
		} else 
		{
            s1 = h * (d + l);
            s2 = h * l;
		}
		LD alpha1 = 2.0 * asin( h / r1 );
		LD alpha2 = 2.0 * asin( h / r2 );
		LD sec1 = alpha1 * r1 * r1 / 2.0;
		LD sec2 = alpha2 * r2 * r2 / 2.0;
		if(fl) 
            square = (sec1 - s1) + (sec2 - s2);
		else 
            square = pi * r2 * r2 - (sec2 - s2) + (sec1 - s1);
	}

	return square;
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t; scanf("%d", &t);
	for(int z = 0; z < t; ++z)
	{
		int n, m; scanf("%d%d", &n, &m);
		vector< pair<LD, LD> > a(n), b(m);
		for(int i = 0; i < n; ++i)
			cin >> a[i].X >> a[i].Y;
		for(int i = 0; i < m; ++i)
			cin >> b[i].X >> b[i].Y;

		vector<LD> r;
		for(int i = 0; i < m; ++i)
		{
			LD s = intersect(a[0].X, a[0].Y, dist(a[0].X, a[0].Y, b[i].X, b[i].Y), 
							 a[1].X, a[1].Y, dist(a[1].X, a[1].Y, b[i].X, b[i].Y));
			r.pb(s);
		}

		printf("Case #%d:", z + 1);
		for(int i = 0; i < m; ++i)
			printf(" %.7lf", (double)r[i]);
		printf("\n");

	}
	return 0;
}
#endif