#include <iostream>
#include <set>
#include <algorithm>
#include <list>
using namespace std;

int main()
{
    int c;
    cin >> c;
    for (int a = 1; a <= c; ++a) {
	int w, h, x0, y0, x1, y1, x, y;
	cin >> w >> h >> x0 >> y0 >> x1 >> y1 >> x >> y;
	long long z = 0;
	if ((long long)x0 * (long long)y1 == (long long)x1 * (long long)y0) {
	    set<pair<int, int> > v;
	    list<pair<int, int> > q;
	    q.push_back(make_pair(x, y));
	    while (!q.empty()) {
		pair<int, int> p = q.front();
		q.pop_front();
		if (a == -66)
		    cout << p.first << ' ' << p.second << endl;
		++z;
		pair<int, int> p0 = make_pair(p.first + x0, p.second + y0);
		if (p0.first >= 0 && p0.first < w &&
		    p0.second >= 0 && p0.second < h &&
		    v.insert(p0).second) {
		    q.push_back(p0);
		}
		pair<int, int> p1 = make_pair(p.first + x1, p.second + y1);
		if (p1.first >= 0 && p1.first < w &&
		    p1.second >= 0 && p1.second < h &&
		    v.insert(p1).second) {
		    q.push_back(p1);
		}
	    }
	} else {
	    int i0 = x, j0 = y, i1 = x, j1 = y;
	    int k = 0;
	    while (true) {
		while (!(i0 >= 0 && i0 < w && j0 >= 0 && j0 < h)) {
		    --k;
		    if (k <= 0) goto done;
		    i0 += x0;
		    j0 += y0;
		}
		while (!(i1 >= 0 && i1 < w && j1 >= 0 && j1 < h)) {
		    --k;
		    i1 -= x0;
		    j1 -= y0;
		}
		while (i1 >= 0 && i1 < w && j1 >= 0 && j1 < h) {
		    ++k;
		    i1 += x0;
		    j1 += y0;
		}
		if (a == -66) {
		    for (int i = i0, j = j0; i != i1 || j != j1;
			 i += x0, j += y0)
			cout << i << ' ' << j << endl;
		}
		z += k;
		i0 += x1;
		j0 += y1;
		i1 += x1;
		j1 += y1;
	    }
	done:
	    ;
	}
	cout << "Case #" << a << ": " << z << endl;
    }
    return 0;
}
