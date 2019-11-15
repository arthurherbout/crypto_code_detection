#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


const int INF = 1000000;

int n, m, f;
string a[50];
int d[50][50][50][50];

bool in (int x, int l, int r) {
	return l <= x && x <= r;
}

bool good (int x, int y, int l, int r) {
	return a[x][y] != '#' || in (y, l, r);
}

bool fall (int x, int y, int & nx) {
	nx = x+1;
	while (nx+1 < n && a[nx+1][y] != '#')
		++nx;
	return nx - x <= f;
}

int get_d (int x, int y, int l, int r) {
	int & my = d[x][y][l][r];
	if (my != -1)  return my;

	if (x == n-1)
		return my = 0;
	if (! good (x, y, l, r))
		return my = INF;

	int ll = y,  rr = y;
	while (ll-1 >= 0 && good (x, ll-1, l, r) && a[x+1][ll-1] == '#')
		--ll;
	while (rr+1 < m && good (x, rr+1, l, r) && a[x+1][rr+1] == '#')
		++rr;

	my = INF;
	if (ll-1 >= 0 && good (x, ll-1, l, r) && a[x+1][ll-1] != '#') {
		int nx, ny = ll-1;
		if (fall (x, ny, nx))
			my = min (my, get_d (nx, ny, 1, 0));
	}
	if (rr+1 < m && good (x, rr+1, l, r) && a[x+1][rr+1] != '#') {
		int nx, ny = rr+1;
		if (fall (x, ny, nx))
			my = min (my, get_d (nx, ny, 1, 0));
	}

	for (int nl=ll; nl<rr; ++nl)
		for (int nr=nl; nr<rr; ++nr) {
			int nx,  ny = nr;
			if (fall (x, ny, nx))
				my = min (my, nr-nl+1 + get_d (nx, ny, nx==x+1 ? nl : 1, nx==x+1 ? nr : 0));
		}
	for (int nr=rr; nr>ll; --nr)
		for (int nl=nr; nl>ll; --nl) {
			int nx,  ny = nl;
			if (fall (x, ny, nx))
				my = min (my, nr-nl+1 + get_d (nx, ny, nx==x+1 ? nl : 1, nx==x+1 ? nr : 0));
		}

	return my;
}

int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	for (int tt=0; tt<ts; ++tt) {
		cin >> n >> m >> f;
		for (int i=0; i<n; ++i)
			cin >> a[i];

		memset (d, -1, sizeof d);
		int ans = get_d (0, 0, 1, 0);
		printf ("Case #%d: ", tt+1);
		if (ans == INF)
			printf ("No\n");
		else
			printf ("Yes %d\n", ans);
	}

}
