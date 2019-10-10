#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std;

int t, cases;
int x, s, r, n;
double _t;
double ret;

struct way
{
	int s, e, v;

	bool operator < (const way& rhs) const
	{
		return s < rhs.s;
	}
};

vector<way> w;

struct comp
{
	bool operator () (const way& lhs, const way& rhs)
	{
		return lhs.v < rhs.v;
	}
};

int main()
{
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d%d%d%lf%d", &x, &s, &r, &_t, &n);

		w.clear();
		way w1;
		int p = 0;
		for (int i = 0; i < n; ++i)
		{
			scanf("%d%d%d", &w1.s, &w1.e, &w1.v);
			if (w1.s != p)
			{
				way w2;
				w2.s = p; w2.e = w1.s; w2.v = 0;
				w.push_back(w2);
			}
			w.push_back(w1);
			p = w1.e;
		}
		if (p != x)
		{
			way w1;
			w1.s = p; w1.e = x; w1.v = 0;
			w.push_back(w1);
		}
		sort(w.begin(), w.end(), comp());

		ret = 0;
		
		for (int i = 0; i < w.size(); ++i)
		{
			double canRunningT = double(w[i].e - w[i].s) / double(w[i].v + r);
			if (_t >= canRunningT)
			{
				ret += canRunningT;
				_t -= canRunningT;
			}
			else
			{
				double canRunningX = _t * double(w[i].v + r);
				ret += _t;
				_t = 0.f;
				double walkingT = double(w[i].e - w[i].s - canRunningX) / double(w[i].v + s);
				ret += walkingT;
			}
		}

		printf("Case #%d: %.10lf\n", ++cases, ret);
	}
}