#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <thread>
#include <mutex>

using namespace std;
#define MOD @
#define ADD(X,Y) ((X) = ((X) + (Y)%MOD) % MOD)
typedef long long i64; typedef vector<int> ivec; typedef vector<string> svec;
typedef long double ld;

struct Worker {
int N;
int X[5050], Y[5050], Z[5050];
ld oxs[5050], oys[5050], ozs[5050];
ld xs[5050], ys[5050], zs[5050];
const ld EPS = 1e-17;
const ld EPS2 = 1e-13;

void cpy()
{
	for (int i = 0; i < N; ++i) {
		xs[i] = oxs[i];
		ys[i] = oys[i];
		zs[i] = ozs[i];
	}
}

pair<ld, ld> rot(ld x, ld y, ld ang)
{
	ld c = cos(ang);
	ld s = sin(ang);
	return {c * x - s * y, s * x + c * y};
}

const ld PI = 3.1415926535897932384626433;
ld normalize(ld ang)
{
	if (ang < 0) ang += 2 * PI;
	if (ang > 2 * PI) ang -= 2 * PI;
	return ang;
}

bool check(int t)
{
	cpy();

	// move P[t] to (1, 0, 0)

	// P[t] -> (*, 0, *)
	if (xs[t] * xs[t] + ys[t] * ys[t] > EPS * EPS) {
		ld ang = -atan2(ys[t], xs[t]);
		for (int i = 0; i < N; ++i) {
			auto xy = rot(xs[i], ys[i], ang);
			xs[i] = xy.first;
			ys[i] = xy.second;
		}
	}
	// P[t] -> (1, 0, 0)
	{
		ld ang = -atan2(zs[t], xs[t]);
		for (int i = 0; i < N; ++i) {
			auto xz = rot(xs[i], zs[i], ang);
			xs[i] = xz.first;
			zs[i] = xz.second;
		}
	}
	/*
	for (int i = 0; i < N; ++i) {
		printf("%f %f %f\n", (double)xs[i], (double)ys[i], (double)zs[i]);
	}
	puts("");
	*/
	int cnt = 0;
	vector<pair<ld, int> > eve;

	for (int i = 0; i < N; ++i) if (i != t) {
		if (ys[i] * ys[i] + zs[i] * zs[i] > EPS * EPS) {
			ld ang = atan2(zs[i], ys[i]);
			ld lf = ang - PI / 2 - EPS2;
			ld rg = ang + PI / 2 + EPS2;
			lf = normalize(lf);
			rg = normalize(rg);
			//printf("%f %f %f\n", (double)ang, (double)lf, (double)rg);

			if (lf > rg) ++cnt;
			eve.push_back({lf, 1});
			eve.push_back({rg, -1});
		}
	}
	sort(eve.begin(), eve.end());
	//for (auto e : eve) printf("%f %d\n", (double)e.first, e.second);
	for (auto e : eve) {
		//printf("%d %d\n", cnt, e.second);
		cnt += e.second;
		if (cnt == 0) return false;
	}
	return true;
}
int gcd(int a, int b)
{
	a = abs(a);
	b = abs(b);
	return b == 0 ? a : gcd(b, a % b);
}
void input()
{
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		scanf("%d%d%d", X + i, Y + i, Z + i);
		int g = gcd(X[i], gcd(Y[i], Z[i]));
		X[i] /= g;
		Y[i] /= g;
		Z[i] /= g;
		ld norm = sqrt((ld)X[i] * X[i] + (ld)Y[i] * Y[i] + (ld)Z[i] * Z[i]);
		oxs[i] = X[i] / norm;
		oys[i] = Y[i] / norm;
		ozs[i] = Z[i] / norm;
	}
}
bool solve()
{
	bool isok = true;
	for (int i = 0; i < N; ++i) {
		if (!check(i)) isok = false;
		//puts("");
	}
	for (int i = 0; i < N; ++i) {
		for (int j = i + 1; j < N; ++j) {
			if (X[i] == -X[j] && Y[i] == -Y[j] && Z[i] == -Z[j]) isok = true;
		}
	}
	return isok;
}
};

int nTC, tcid;
mutex mtx;
bool ans[210];

void work()
{
	Worker wk;
	for (;;) {
		mtx.lock();
		if (tcid >= nTC) {
			mtx.unlock();
			return;
		}
		wk.input();
		int myid = tcid++;
		mtx.unlock();
		ans[myid] = wk.solve();
	}
}

int gcd(int a, int b)
{
	a = abs(a);
	b = abs(b);
	return b == 0 ? a : gcd(b, a % b);
}
int main()
{
	scanf("%d", &nTC);
	tcid = 0;
	vector<thread> ths;
	for (int i = 0; i < 10; ++i) ths.push_back(thread(work));
	for (int i = 0; i < 10; ++i) ths[i].join();

	for (int i = 0; i < nTC; ++i) {
		printf("Case #%d: %s\n", i + 1, ans[i] ? "YES" : "NO");
	}
	return 0;
}

