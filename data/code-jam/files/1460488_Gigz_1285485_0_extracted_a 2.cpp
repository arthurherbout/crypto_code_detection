#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <hash_map>
#include <hash_set>
#include <set>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))

FILE *fi, *fo;

struct Point
{
	int x, y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	bool operator < (const Point& a) const
	{
		return (x < a.x) || ((x == a.x) && (y < a.y));
	}
};

struct Wall
{
	Point a, b;
};

int distance2(const Point& a, const Point& b)
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

bool isCloseEnough(const Point& a, const Point& b, int d)
{
	return (distance2(a, b) <= d*d);
}

Point createVector(const Point& b, const Point& e)
{
	Point res;

	res.x = e.x - b.x;
	res.y = e.y - b.y;

	return res;
}

int crossProd(const Point& a, const Point& b)
{
	return a.x*b.y - a.y*b.x;
}

bool isBetween(int x, int a, int b)
{
	return (x >= a && x <= b && b >= a) || (x >= b && x <= a && a >= b);
}

bool isVisible(Point& a, Point& b, set<Point>& points)
{
	for(set<Point>::iterator it = points.begin(); it != points.end(); ++it)
	{
		if (it->x == a.x && it->y == a.y) continue;
		if (it->x == b.x && it->y == b.y) continue;
		if (isBetween(it->x, a.x, b.x) && isBetween(it->y, a.y, b.y) && crossProd(createVector(a, *it), createVector(a, b)) == 0) return false;
	}	
	return true;
}

Point reflect(Point& a, Wall& w)
{
	if (crossProd(createVector(w.a, w.b), createVector(w.a, a)) == 0) return Point(a);
	if (w.a.x == w.b.x) return Point(w.a.x + (w.a.x - a.x), a.y);
	if (w.a.y == w.b.y) return Point(a.x, w.a.y + (w.a.y - a.y));
	return Point(a);
}

set<Point> S;
vector<Point> Q;

void mainImpl(int _c)
{
	int n, m, d;
	fscanf(fi, "%d%d%d\n", &n, &m, &d);
	Point w0(0,0), w1((n-2)*2, 0), w2(0, (m-2)*2), w3((n-2)*2, (m-2)*2);
	Wall W[4];
	W[0].a = w0; W[0].b = w1;
	W[1].a = w0; W[1].b = w2;
	W[2].a = w3; W[2].b = w1;
	W[3].a = w3; W[3].b = w2;
	int i, j;
	Point A;
	FOR(i, n) 
	{
		FOR(j, m) 
		{
			char c;
			fscanf(fi, "%c", &c);
			if (c == 'X')
			{
				A.x = i*2 - 1;
				A.y = j*2 - 1;
			}
		}
		fscanf(fi, "\n");
	}

	d *= 2;

	Q.clear();
	S.clear();

	S.insert(A);
	Q.push_back(A);
	int qb = 0;
	while (qb < Q.size())
	{
		FOR(i, 4)
		{
			Point p = reflect(Q[qb], W[i]);
			if (isCloseEnough(A, p, d) && S.find(p) == S.end())
			{
				Q.push_back(p);
				S.insert(p);
			}
		}
		++qb;
	}

	int res = 0;
	for (i = 1; i < Q.size(); ++i)
	{
		if (isVisible(A, Q[i], S)) ++res;
	}

	fprintf(fo, "Case #%d: %d\n", _c, res);
}


int main()
{
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	int t, tt;
	fscanf(fi, "%d\n", &tt);

	FOR(t, tt) mainImpl(t + 1);

	fclose(fo);
	fclose(fi);
	return 0;
}