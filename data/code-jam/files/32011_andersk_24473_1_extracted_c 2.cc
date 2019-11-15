#include <iostream>
using namespace std;

bool m[51][51];
int n[50][50];
int r, c;
int Y;

void dump()
{
    for (int j = 1; j <= r; ++j) {
	for (int i = 1; i <= c; ++i)
	    cout << (m[i][j] ? '#' : char('0' + n[i][j]));
	cout << endl;
    }
    cout << endl;
}

bool compute(int x, int y)
{
    int nn = n[x-1][y-1] -
	(m[x-2][y-2] + m[x-2][y-1] + m[x-2][y] +
	 m[x-1][y-2] + m[x-1][y-1] + m[x-1][y] +
	 m[x][y-2] + m[x][y-1]);
    if (nn == 0)
	m[x][y] = false;
    else if (nn == 1)
	m[x][y] = true;
    else {
	//cout << "FAIL1 " << x << " " << y << endl;
	//dump();
	return false;
    }
    return true;
}

bool check(int x, int y)
{
    bool ret = (m[x-1][y-1] + m[x-1][y] + m[x-1][y+1] +
		m[x][y-1] + m[x][y] + m[x][y+1] +
		m[x+1][y-1] + m[x+1][y] + m[x+1][y+1]) == n[x][y];
    if (!ret) {
	//cout << "FAIL2 " << x << " " << y << endl;
	//dump();
    }
    return ret;
}

void search(int x, int y);

void searchx(int x, int y)
{
    for (int j = 2; j <= y; ++j)
	if (!compute(x, j))
	    return;
    search(x, y);
}

void searchy(int x, int y)
{
    for (int i = 2; i <= x; ++i)
	if (!compute(i, y))
	    return;
    search(x, y);
}

void search(int x, int y)
{
    //cout << "search " << x << ' ' << y << endl;
    //dump();
    if (x >= c && y >= r) {
	for (int i = 1; i <= c; ++i)
	    if (!check(i, r))
		return;
	for (int j = 1; j < r; ++j)
	    if (!check(c, j))
		return;
	int j = (r + 1) / 2;
	int YY = 0;
	for (int i = 1; i <= c; ++i)
	    if (m[i][j])
		++YY;
	//cout << "FOUND " << YY << endl;
	//dump();
	if (Y < YY)
	    Y = YY;
	return;
    }
    if (x <= y && x < c || y >= r) {
	++x;
	m[x][1] = true;
	searchx(x, y);
	m[x][1] = false;
	searchx(x, y);
    } else {
	++y;
	m[1][y] = true;
	searchy(x, y);
	m[1][y] = false;
	searchy(x, y);
    }
}

int main()
{
    int N;
    cin >> N;
    for (int X = 1; X <= N; ++X)
    {
	cin >> r >> c;
	for (int y = 0; y <= r + 1; ++y)
	    for (int x = 0; x <= c + 1; ++x)
		m[x][y] = false;
	for (int y = 1; y <= r; ++y)
	    for (int x = 1; x <= c; ++x)
		cin >> n[x][y];
	Y = -1;
	m[1][1] = true;
	search(1, 1);
	m[1][1] = false;
	search(1, 1);
	cout << "Case #" << X << ": " << Y << endl;
    }
    return 0;
}
