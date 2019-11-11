#include <iostream>
#include <queue>

using namespace std;

const int MAX = 520;

int f[MAX][MAX];
int h[MAX][MAX];
int v[MAX][MAX];
int ans[MAX];
bool a[MAX][MAX];
bool w[MAX][MAX];

const bool converter[16][4] = {{0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},{0,1,0,1},{0,1,1,0},{0,1,1,1},
			       {1,0,0,0},{1,0,0,1},{1,0,1,0},{1,0,1,1},{1,1,0,0},{1,1,0,1},{1,1,1,0},{1,1,1,1}};

void convert(int i, int j, char c) {
	j = j*4;
	int tc;
	if (c >= 'A' && c <= 'F')
		tc = c-'A'+10;
	else
		tc = c-'0';
	for (int k = 0; k < 4; ++k)
		a[i][j+k] = converter[tc][k];
}

struct item
{
	int i, j, size;
	item(int _i = 0, int _j = 0, int _size = 0) : i(_i), j(_j), size(_size) {}
	friend bool operator < (item it1, item it2) {
		if (it1.size == it2.size && it1.i == it2.i)
			return it1.j > it2.j;
		if (it1.size == it2.size)
			return it1.i > it2.i;
		return it1.size < it2.size;
	}
};

priority_queue<item> q;

int main()
{                
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
               
	int t;
	cin >> t; int m, n; char c;

	for (int tt = 0; tt < t; ++tt) {
		memset(f, 0, sizeof(f));
		memset(h, 0, sizeof(h));
		memset(v, 0, sizeof(v));
		memset(w, 0, sizeof(w));
		memset(ans, 0, sizeof(ans));
		cin >> n >> m;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m/4; ++j) {
				cin >> c;
				convert(i, j, c);
			}
		}
/*		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j)
				cout << field[i][j];
			cout << endl;
		}*/

		h[0][0] = 1;
		v[0][0] = 1;
		f[0][0] = 1;

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				if (j == 0)
					h[i][j] = 1;
				else {
					if (a[i][j-1] != a[i][j])
						h[i][j] = h[i][j-1]+1;
					else
						h[i][j] = 1;
				}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				if (i == 0)
					v[i][j] = 1;
				else {
					if (a[i-1][j] != a[i][j])
						v[i][j] = v[i-1][j]+1;
					else
						v[i][j] = 1;
				}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				if (i == 0 || j == 0)
					f[i][j] = 1;
				else {
					if (a[i][j] == a[i-1][j-1])
						f[i][j] = min(f[i-1][j-1]+1, min(h[i][j], v[i][j]));
					else
						f[i][j] = 1;
				}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				q.push(item(i, j, f[i][j]));
		int maxIndex = q.top().size;
		while (!q.empty()) {
			item it = q.top(); q.pop();
			if (!w[it.i][it.j] && !w[it.i-it.size+1][it.j-it.size+1] && !w[it.i-it.size+1][it.j] && !w[it.i][it.j-it.size+1]) {
				++ans[it.size];
				for (int i = 0; i < it.size; ++i)
					for (int j = 0; j < it.size; ++j)
						w[it.i-i][it.j-j] = 1;
			}
			else
				if (it.size != 1)
					q.push(item(it.i, it.j, it.size-1));
		}
		int cAns = 0;
		for (int i = maxIndex; i >= 1; --i)
			if (ans[i])
				++cAns;
		cout << "Case #" << tt+1 << ": " << cAns << endl;

		for (int i = maxIndex; i >= 1; --i)
			if (ans[i])
				cout << i << ' ' << ans[i] << endl;
	}

	return 0;
}