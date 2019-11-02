#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>

using namespace std;

const long double eps = 1e-9;



long double x[6], y[6];

void Load()
{
	int i;
	for (i = 0; i < 6; i++) cin >> x[i] >> y[i];
}

long double Dist(int i, int j)
{
	return sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
}
     

bool Inside(long double xx, long double yy)
{
	int i, j;
	long double t1, t2;
	for (i = 0; i < 3; i++) {
		long double a, b, c;
		a = y[i + 3] - y[(i + 1) % 3 + 3];
		b = -x[i + 3] + x[(i + 1) % 3 + 3];
		c = - a * x[i + 3] - b * y[i + 3];
		t1 = xx * a + yy * b + c;
		t2 = x[(i + 2) % 3 + 3] * a + y[(i + 2) % 3 + 3] * b + c;
		if (t1 * t2 < -eps) return false;
	}
	return true;
}

long double t, ang, xx0,  yy0;

bool Check(long double xx, long double yy)
{
	long double xxx, yyy;
	xxx = (xx * cos(ang) + yy * sin(ang)) * t + xx0;
	yyy = (xx * (-sin(ang)) + yy * cos(ang)) * t + yy0;
	if (fabs(xxx - xx) + fabs(yyy - yy) > eps) return false; 

	return true;
}

void Solve()
{


	t = Dist(3,4) / Dist(0,1);
	long double vx1, vy1, vx2, vy2;
	vx1 = x[1] - x[0];
	vy1 = y[1] - y[0];
	vx2 = x[4] - x[3];
	vy2 = y[4] - y[3];
	ang = atan2(vx2 * vy1 - vy2 * vx1, vx1 * vx2 + vy1 * vy2);
	long double x0, y0;
	xx0 =  x0 = x[3] - (x[0] * cos(ang) + y[0] * sin(ang)) * t;
	yy0 = y0 = y[3] - (x[0] * (-sin(ang)) + y[0] * cos(ang)) * t;
	long double a1, b1, a2, b2, c1, c2;

	
	a1 = cos(ang);
	a2 = (-sin(ang));
	b1 = sin(ang);
	b2 = cos(ang); a1 *= t; b1 *= t; a2 *= t; b2 *= t;
	a1 -= 1;
	b2 -= 1;
	c1 = -x0;
	c2 = -y0;
	long double xx, yy;
	xx = (x[2] * cos(ang) + y[2] * sin(ang)) * t + x0;
	yy = (x[2] * (-sin(ang)) + y[2] * cos(ang)) * t + y0;

	long double d, d1, d2;
	d = a1 * b2 - b1 * a2;
	d1 = c1 * b2 - c2 * b1;
	d2 = a1 * c2 - c1 * a2;
	cout.setf(ios::fixed|ios::showpoint);
	cout.precision(6);
	if (fabs(d) + fabs(d1) + fabs(d2) < 3 * eps) {
		cout << x[3] << " " << y[3] << "\n";
		if (!Check(x[3], y[3])) {
			cout << " !!!\n";
			return;
 
		}

		return;
	}
	if (fabs(d) < eps) {
		cout << " No Solution\n";
		return;
	}
	x0 = d1 / d;
	y0 = d2 / d;
	if (!Inside(x0, y0)) {
		cout << " No Solution\n";
		return;

	}
	if (!Check(x0, y0)) {
		cout << " !!!\n";
		return;
 
	}

	cout << x0 << " " << y0 << "\n";
}


int main()
{
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int tt, t;
	cin >> tt;
	for (t = 1; t <= tt; t++) {
		cout << "Case #" << t << ": ";
		Load();
		Solve();
	}
	return 0;
}