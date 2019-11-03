#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#define PI 3.1415926535897932

using namespace std;

int n, m;
int x[5000], y[5000];
int a[5000], b[5000];

const int mx[4] = {1, 0, -1,  0};
const int my[4] = {0, 1,  0, -1};

struct event_t
{
	int x, y;
} data[8000], data2[8000];
int dp, dp2, dpbase;
double ans;

double angles[90000];
int ap;

bool operator < (const event_t& a, const event_t& b)
{
	return (a.x<b.x || a.x==b.x&&a.y<b.y);
}

double normalized (double angle)
{
	while (angle < -PI)
		angle  += PI * 2;
	while (angle > PI)
		angle -= PI * 2;
	return angle;
}

void getBest (double angle, bool good)
{
	double xx[4], yy[4];

	for (int i = 0; i < 4; i ++)
		xx[i] = yy[i] = 0;

	for (int i = 0; i < 4; i ++)
	{
		int id = 0;
		long long len = cos(angle) * (long long)(a[0] * mx[i] + b[0] * my[i]) + sin(angle) * (long long)(a[0] * mx[(i + 1) % 4] + b[0] * my[(i + 1) % 4]);
		for (int j = 1; j < n; j ++)
		{
			long long cur = cos(angle) * (long long)(a[j] * mx[i] + b[j] * my[i]) + sin(angle) * (long long)(a[j] * mx[(i + 1) % 4] + b[j] * my[(i + 1) % 4]);

			if (cur > len)
			{
				len = cur;
				id = j;
			}
		}

		for (int j = 0; j < 4; j ++)
		{
			int mul = (std::max(m - j, 0) + (3-i)) / 4;
			xx[j] += mul * (double)(a[id] * mx[i] + b[id] * my[i]);
			yy[j] += mul * (double)(a[id] * mx[(i + 1) % 4] + b[id] * my[(i + 1) % 4]);
		}
	}

	for (int j = 0; j < 4; j ++)
	{
//		printf ("getBest %lf: %lf\n", angle, xx[j]*xx[j]+yy[j]*yy[j]);
		if (sqrt (xx[j] * xx[j] + yy[j] * yy[j]) > ans)
		{
			if (!good)
			{
//				printf ("Oops %lf->%lf\n", ans, sqrt (xx[j] * xx[j] + yy[j] * yy[j]));
//				exit(0);
			}
			ans = sqrt (xx[j] * xx[j] + yy[j] * yy[j]);
		}
	}
}

void maintain ()
{
	while (dp2-2 > dpbase && (data2[dp2 - 1].x-data2[dp2 - 3].x)*(data2[dp2 - 2].y-data2[dp2 - 3].y) >= (data2[dp2 - 1].y-data2[dp2 - 3].y)*(data2[dp2 - 2].x-data2[dp2 - 3].x))
	{
//		printf ("%d %d %d %d %d %d : REMOVE\n", data2[dp2-3].x,data2[dp2-3].y, data2[dp2-2].x,data2[dp2-2].y, data2[dp2-1].x,data2[dp2-1].y);
		data2[dp2 - 2] = data2[dp2 - 1];
		dp2 --;
	}
}

int main ()
{
	int t, ct = 0;

	for (scanf ("%d", &t); t > 0; t --)
	{
		scanf ("%d%d", &n, &m);

		dp = 0;

		for (int i = 0; i < n; i ++)
		{
			scanf ("%d%d", x + i, y + i);

			a[i] = x[i] - y[i];
			b[i] = x[i] + y[i];

			data[dp].x = a[i];
			data[dp].y = b[i];
			dp ++;
		}

		sort(data, data + dp);

		dp2 = 0;
		dpbase = 0;
		for (int i = 0; i < dp; i ++)
		{
			data2[dp2 ++] = data[i];
			maintain ();
		}
		dpbase = dp2 - 1;
		for (int i = dp - 2; i >= 0; i --)
		{
			data2[dp2 ++] = data[i];
			maintain ();
		}

		ap = 0;
		for (int i = 0; i + 1 < dp2; i ++)
		{
			int j = (i + 1) % dp2;

			angles[ap ++] = atan2 (data2[i].x - data2[j].x, (double)data2[j].y - data2[i].y);
			angles[ap ++] = atan2 (data2[i].y, (double)data2[i].x);
		}

/*
		printf ("dp2 = %d, ap = %d\n", dp2, ap);
		for (int i = 0; i < dp2; i ++)
			printf ("dp2: %d %d\n", data2[i].x, data2[i].y);
*/
		for (int i = 0; i < ap; i ++)
		{
//			printf ("Angle: %lf\n", angles[i]);
			angles[i] = normalized(angles[i]);
			angles[i + ap] = normalized (angles[i] + PI / 2);
			angles[i + ap * 2] = normalized (angles[i] + PI);
			angles[i + ap * 3] = normalized (angles[i] + PI * 3/ 2);
		}
		ap *= 4;
		sort(angles, angles + ap);

		ans = 0;

		for (int i = 0; i < ap - 1; i ++)
			::getBest ((angles[i + 1] + angles[i]) / 2, true);
		::getBest((angles[0] + angles[ap - 1]) / 2, true);
		::getBest((angles[0] + angles[ap - 1] + PI * 2) / 2, true);


		for (double xx = 0; xx <= 6.30; xx += 6.30 / n)
			::getBest ( xx, false);

		printf ("Case #%d: %.10lf\n", ++ ct, ans);
		fprintf (stderr, "Case #%d: %.10lf\n", ct, ans);
	}

	return 0;
}