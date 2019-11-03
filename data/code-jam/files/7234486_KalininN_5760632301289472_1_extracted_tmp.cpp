#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using uint = unsigned int;
using D = double;
using ii = pair<int, int>;

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

#define y0 y0_sadf

D y0;

pair<D, D> calc(D y, D ys)
{
	D x = -10;
	D dx = 1.0 / 10000;
	D ans = 0;
	while (x < 10)
	{
// 		if (y > 100) return {1e100, 100};
// 		if (y < -100) return {1e100, -100};
		ans += (1 + 1.0 / (x * x + y * y)) * sqrt(1 + ys * ys) * dx;
		x += dx;
		D yss = (1 + ys * ys) * (2 * x * ys - 2 * y) / (x * x + y * y + 1) / (x * x + y * y);
// 		cout << x << ' ' << y << ' ' << ys << ' ' << yss << endl;
		y += ys * dx;
		ys += yss * dx;
	}
	return {ans, y};
}

pair<D, D> calc2(D y, D ys)
{
	D x = -10;
	D dx = 1.0 / 10000;
	D ans = 0;
	while (x < 10)
	{
				if (y > 100) return {1e100, 100};
				if (y < -100) return {1e100, -100};
		ans += (1 + 1.0 / (x * x + y * y) + 1.0 / (x * x + (y - y0) * (y - y0))) * sqrt(1 + ys * ys) * dx;
		x += dx;
		D s2 = 1 + ys * ys;
		D d = x * x + y * y;
		D d2 = x * x + (y - y0) * (y - y0);
		D left = s2 * (-2 * y) / d / d - s2 * 2 * (y - y0) / d2 / d2 + ys * ((2 * x + 2 * y * ys) / d / d + (2 * x + 2 * (y - y0) * ys) / d2 / d2);
		D right = (1 + 1 / d + 1 / d2) / s2;
		D yss = left / right;
		// 		cout << x << ' ' << y << ' ' << ys << ' ' << yss << endl;
		y += ys * dx;
		ys += yss * dx;
	}
	return {ans, y};
}

int main()
{
	int NT;
	scanf("%d", &NT);
// 	NT = 1;
	for (int T = 1; T <= NT; T++)
	{
		printf("Case #%d:", T);
		
		int n;
		D a, b, pos;
		cin >> n >> a >> b;
		cin >> pos;
		a -= pos;
		b -= pos;
		if (n == 1)
		{
		D answer = 1e100;
		{
			D l = -6000.0 * abs(b - a) / 20.0;
			D r = -a / 10.0;
// 			cout << l << ' ' << r << endl;
			for (int IT = 0; IT < 100; IT++)
			{
				D m = (l + r) / 2;
				D t = calc(a, m).se;
// 				cout << "try " << a << ' ' << m << ' ' << t << endl;
				if (t < b) l = m;
				else r = m;
			}
			auto t = calc(a, l);
// 			cout << t.fi << ' ' << t.se << endl;
			answer = t.fi;
		}
		{
			D l = -a / 10.0;
			D r = 6000 * abs(b - a) / 20.0;
// 			cout << l << ' ' << r << endl;
			for (int IT = 0; IT < 100; IT++)
			{
				D m = (l + r) / 2;
				D t = calc(a, m).se;
// 				cout << "try " << a << ' ' << m << ' ' << t << endl;
				if (t < b) l = m;
				else r = m;
			}
			auto t = calc(a, l);
// 			cout << t.fi << ' ' << t.se << endl;
			answer = min(answer, t.fi);
		}
		
		printf(" %.7lf\n", answer);
		} else
		{
			cin >> y0;
			y0 -= pos;
			D answer = 1e100;
			{
				D l = -10000.0 * abs(b - a) / 20.0;
				D r = -max(a, a - y0) / 10.0;
				// 			cout << l << ' ' << r << endl;
				for (int IT = 0; IT < 100; IT++)
				{
					D m = (l + r) / 2;
					D t = calc2(a, m).se;
					// 				cout << "try " << a << ' ' << m << ' ' << t << endl;
					if (t < b) l = m;
					else r = m;
				}
				auto t = calc2(a, l);
				// 			cout << t.fi << ' ' << t.se << endl;
				answer = t.fi;
			}
			{
				D l = -max(a, a - y0) / 10.0;
				D r = -min(a, a - y0) / 10.0;
// 							cout << l << ' ' << r << endl;
				for (int IT = 0; IT < 100; IT++)
				{
					D m = (l + r) / 2;
					D t = calc2(a, m).se;
// 									cout << "try " << a << ' ' << m << ' ' << t << endl;
					if (t < b) l = m;
					else r = m;
				}
				auto t = calc2(a, l);
				// 			cout << t.fi << ' ' << t.se << endl;
				answer = min(answer, t.fi);
			}
			{
				D l = -min(a, a - y0) / 10.0;
				D r = 6000 * abs(b - a) / 20.0;
				// 			cout << l << ' ' << r << endl;
				for (int IT = 0; IT < 100; IT++)
				{
					D m = (l + r) / 2;
					D t = calc2(a, m).se;
					// 				cout << "try " << a << ' ' << m << ' ' << t << endl;
					if (t < b) l = m;
					else r = m;
				}
				auto t = calc2(a, l);
				// 			cout << t.fi << ' ' << t.se << endl;
				answer = min(answer, t.fi);
			}
			
// 			auto t = calc2(a, 0);
// 			cout << t.fi << ' ' << t.se << endl;
			
			printf(" %.7lf\n", answer);
		}
		
		fprintf(stderr, "%d/%d tests completed!\n", T, NT);
	}

}
