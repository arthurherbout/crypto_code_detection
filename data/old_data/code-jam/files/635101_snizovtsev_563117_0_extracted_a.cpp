#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;

#define foreach(iter, cont) \
	for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)

struct Dir {
	map <string, Dir*> child;
};

int putdir (Dir* d, const string path)
{
	int res = 0;
	uint i;
	for (i = 0;  i < path.length (); ) {
		assert (path[i] == '/');
		uint j = i + 1;
		while (j < path.length () && path[j] != '/')
			j++;

		string name = path.substr (i + 1, j - i - 1);
		if (d->child.find (name) != d->child.end ()) {
			d = d->child[name];
		} else {
			Dir* p = new Dir;
			d->child[name] = p;
			d = p;
			res++;
		}

		i = j;
	}
	return res;
}

void dirfree (Dir* d)
{
	foreach (i, d->child)
		dirfree (i->second);
	delete d;
}

int solve ()
{
	Dir* root = new Dir;
	string path;
	int n, m;
	int res = 0;

	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		cin >> path;
		putdir (root, path);
	}
	for (int i = 0; i < m; i++) {
		cin >> path;
		res += putdir (root, path);
	}

	dirfree (root);

	return res;
}

int main ()
{
	int T;
	cin >> T;
	for (int i = 1; i <= T; i++) {
		cout <<  "Case #" << i << ": " << solve () << endl;
	}
	return 0;
}
