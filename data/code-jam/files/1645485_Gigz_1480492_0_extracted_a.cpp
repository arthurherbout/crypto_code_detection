#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <hash_map>
#include <hash_set>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))

FILE *fi, *fo;

struct Event
{
	int Code;
	double Time;
};

struct Car
{
	char Lane;
	double Speed;
	double Position;
};

int n;

Car C[51];

void recalcPositions(double delta)
{
	int i;
	FOR(i, n) C[i].Position += delta * C[i].Speed;
}

Car shift(Car a, double t)
{
	Car ret;
	ret.Lane = a.Lane;
	ret.Speed = a.Speed;
	ret.Position = a.Position + a.Speed * t;

	return ret;
}

bool overlap(const Car& a, const Car& b)
{
	if (a.Position >= b.Position && a.Position < b.Position + 5.0) return true;
	if (b.Position >= a.Position && b.Position < a.Position + 5.0) return true;
	if (a.Position == b.Position + 5.0 && a.Speed < b.Speed) return true;
	if (b.Position == a.Position + 5.0 && b.Speed < a.Speed) return true;
	return false;
}

double notOverlapTime(const Car& a, const Car& b)
{
	if (fabs(a.Speed - b.Speed) < 1e-10) return -1;
	if (a.Speed > b.Speed) return (b.Position + 5.0 - a.Position) / (a.Speed - b.Speed);
	return (a.Position + 5.0 - b.Position) / (b.Speed - a.Speed);
}

Event collision(double time)
{
	Event ret;
	ret.Code = -1;
	ret.Time = time;

	return ret;
}

double nextCollision(const Car& a, const Car& b)
{
	if (a.Speed <= b.Speed) return -1;
	return (a.Position + 5.0 - b.Position) / (b.Speed - a.Speed);
}

Event nextEvent(Event e)
{
	int i, j;
	int a, b;

	double nt = -1.0;
	int code = 0;

	// Try to get back everyone to the left lane
	FOR(i, n) if (C[i].Lane == 'R')
	{
		bool canDo = true;
		FOR(j, n) if (i != j && overlap(C[i], C[j]) && C[j].Lane == 'L')
		{
			canDo = false;
			double ct = notOverlapTime(C[i], C[j]);
			if (fabs(ct) < 1e-10){
				ct = 1e-10;
			}
			if ((nt < 0 || nt > ct) && ct >= 0)
			{
				nt = ct;
				code = 1;
			}
		}
		if (canDo)
		{
			C[i].Lane = 'L';
		}
	}

	FOR(i, n) FOR(j,n) if (i != j && C[i].Lane == C[j].Lane && C[i].Position <= C[j].Position)
	{
		double ct = nextCollision(C[i], C[j]);

		if ((nt < 0 || nt > ct) && ct >= 0)
		{
			nt = ct;
			code = 2;
			a = i;
			b = j;
		}

	}

	code = code;

	if (code == 2)
	{
		if (C[a].Lane == 'R') return collision(e.Time + nt);
		FOR(i, n) if (i != a && C[i].Lane == 'R' && overlap(shift(C[i], nt), shift(C[a], nt))) return collision(e.Time + nt);

		C[a].Lane = 'R';
	}
	
	Event ret;
	ret.Time = e.Time + nt;
	ret.Code = code;

	recalcPositions(ret.Time - e.Time);
	return ret;
}

void mainImpl(int _c)
{
	cout<<_c<<endl;
	int i, j;
	fscanf(fi, "%d\n", &n);
	FOR(i, n) fscanf(fi, "%c%lf%lf\n", &C[i].Lane, &C[i].Speed, &C[i].Position);

	Event e;
	e.Code = 100;
	e.Time = 0;

	while (e.Code != 0 && e.Code != -1)
	{
		e = nextEvent(e);
	}

	fprintf(fo, "Case #%d: ", _c);

	if (e.Code == -1) fprintf(fo, "%.9lf\n", e.Time);
	else if (e.Code == 0) fprintf(fo, "Possible\n");
}


int main()
{
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	int t, tt;
	fscanf(fi, "%d", &tt);

	FOR(t, tt) mainImpl(t + 1);

	fclose(fo);
	fclose(fi);
	return 0;
}