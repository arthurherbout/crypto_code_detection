#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l;

const double Eps = 1e-8;
const double Pi = 3.1415926535897932384626433832795;

bool Eq(double x, double y) {
	return fabs(x-y) < Eps;
}

struct Point {
	double x, y;
	Point() : x(0), y(0) {}
	Point(double x, double y) : x(x), y(y){}
	void read() { cin >> x >> y; }
	double atan() { return atan2(y, x); }
	double len() { return hypot(x, y); }
	bool operator == (const Point &a) const {
		return Eq(x, a.x) && Eq(y, a.y);
	} 
	bool operator != (const Point &a) const {
		return !Eq(x, a.x) || !Eq(y, a.y);
	} 
};

Point operator * (double c, Point p) { return Point(c*p.x,c*p.y); }
Point operator * (Point p, double c) { return Point(c*p.x,c*p.y); }
Point operator + (Point p, Point q) { return Point(p.x+q.x,p.y+q.y); }
Point operator - (Point p, Point q) { return Point(p.x-q.x,p.y-q.y); }
double operator ^ (Point p, Point q) { return p.x * q.y - p.y * q.x; } 

double cw(double x1, double y1, double x2, double y2, double x3, double y3) {
	return (x1-x2)*(y1-y3)-(x1-x3)*(y1-y2);
}

double cw(Point a, Point b, Point c) {
	return (b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);
}

double dist(double x1, double y1, double x2, double y2) {
	return hypot(x2 - x1, y2 - y1);
}

double dist(Point P1, Point P2) {
	return hypot(P2.x - P1.x, P2.y - P1.y);
}

// not precise
double area(double a, double b, double c) {
	double p = (a+b+c) / 2;
	double s = sqrt(p * (p-a) * (p-b) * (p-c));
	return s;
}

void setCircumCenter(double x1, double y1, double x2, double y2, double x3, double y3, double& x, double& y) {
	double A1 = x1-x2;
	double A2 = x2-x3;
	double B1 = y1-y2;
	double B2 = y2-y3;
	double C1 = (A1*(x1+x2) + B1*(y1+y2))/2;
	double C2 = (A2*(x2+x3) + B2*(y2+y3))/2;
	double d = A1*B2-A2*B1;
	if (Eq(d, 0)) {
		x = y = 0.0;
		return;
	}
	y = (A1*C2-A2*C1)/d;
	x = -(B1*C2-B2*C1)/d;
}

void setCircumCenter(Point p1, Point p2, Point p3, Point& p) {
	double A1 = p1.x-p2.x;
	double A2 = p2.x-p3.x;
	double B1 = p1.y-p2.y;
	double B2 = p2.y-p3.y;
	double C1 = (A1*(p1.x+p2.x) + B1*(p1.y+p2.y))/2;
	double C2 = (A2*(p2.x+p3.x) + B2*(p2.y+p3.y))/2;
	double d = A1*B2-A2*B1;
	if (Eq(d, 0)) {
		p.x = p.y = -128.0;
		return;
	}
	p.y = (A1*C2-A2*C1)/d;
	p.x = -(B1*C2-B2*C1)/d;
}

struct line {
    double A, B, C;
	line() { A = 1; B = 0; C = 0; }
    line(Point P1, Point P2) {
        A = P1.y-P2.y;
        B = P2.x-P1.x;
        C = -(A*P1.x+B*P1.y);
    }
    line(line L, Point P) {
        A = L.A;
        B = L.B;
        C = -(A*P.x+B*P.y);
    }
};

Point inter(line L1, line L2) {
    double d = L1.A*L2.B-L1.B*L2.A;
    double d1 = L1.B*L2.C-L2.B*L1.C;
    double d2 = L1.C*L2.A-L2.C*L1.A;
    return Point(d1/d, d2/d);
}

// rotate B about A by alp angle
Point rotate(Point A, Point B, double alp) {
	Point C = B - A, D;
	D.x = C.x * cos(alp) - C.y * sin(alp);
	D.y = C.x * sin(alp) + C.y * cos(alp);
	return D + A;
}

Point P[3], Q[3], R[3];
int id[3];
double coeff, c2;

void get(Point X, Point A, Point B, double& alp, double& d) {
	if (A == X) {
		d = 0;
		alp = 0;
		return;
	}

	d = (X-A).len() / (B-A).len();

	Point p1 = B-A;
	Point p2 = X-A;

	alp = atan2( p1^p2, p1.x*p2.x+p1.y*p2.y);
}

// The first problem. Should be the easiest.
int main() {

//	freopen("x.in", "r", stdin);

//	freopen("A-small-attempt0.in", "r", stdin);
//	freopen("A-small-attempt0.out", "w", stdout);

	freopen("A-large.in", "r", stdin);
	freopen("A-large2.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		printf("Case #%d: ", tt);

		F0(i,3) P[i].read();
		F0(i,3) Q[i].read();

		double a1, d1, a2, d2, a3, d3;

		get(Q[0], P[0], P[1], a1, d1);
		get(Q[1], P[1], P[2], a2, d2);
		get(Q[2], P[2], P[0], a3, d3);

		int u;

		F0(i,3) Q[i] = P[i];
		F0(u,1000000) {
			R[0] = rotate(Q[0], Q[1], a1);
			R[0] = (R[0] - Q[0]) * d1 + Q[0];
			R[1] = rotate(Q[1], Q[2], a2);
			R[1] = (R[1] - Q[1]) * d2 + Q[1];
			R[2] = rotate(Q[2], Q[0], a3);
			R[2] = (R[2] - Q[2]) * d3 + Q[2];
			F0(i,3) Q[i] = R[i];

			double d = (Q[0]-Q[1]).len();
			d = max(d, (Q[1]-Q[2]).len());
			d = max(d, (Q[0]-Q[2]).len());
			if (d < 1e-12) break;
		}
	
		printf("%.6lf %.6lf", Q[0].x, Q[0].y);
		printf("\n");
	}
	
	return 0;
}
