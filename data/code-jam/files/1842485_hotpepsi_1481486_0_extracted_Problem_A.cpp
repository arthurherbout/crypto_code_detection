/*

Google Code Jam 2012 Round 2

Problem A. 


*/

#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

typedef pair<int, int> II;
typedef vector<int> IntVec;
typedef vector<II> IIVec;
typedef set<II> IISet;
typedef map<int, II> IIMap;
typedef map<int, int> IntMap;
typedef queue<II> IIQueue;

//static int f[1000000004];

bool solve(int N, int D, IIVec &v) {
	IISet memo;
//	memset(f, 0, sizeof(f));
	IntMap m;
	int i;
	for (i = 0; i < N; ++i) {
		m[v[i].first] = v[i].second;
	}
	IIQueue q;
	q.push(II(v[0].first, v[0].first));
	while (q.size() > 0) {
		II x = q.front();
		q.pop();
//		if (M[x.first] >= x.second) {
//			continue;
//		}
//		M[x.first] = x.second;
		memo.insert(x);
		int l = max(0, x.first - x.second);
		int h = min(D, x.first + x.second);
		if (h >= D) {
			return true;
		}
		IntMap::const_iterator low = m.lower_bound(l);
//		IntMap::const_iterator high = m.upper_bound(h);
		IntMap::const_iterator it;
//		for (it = low; it != high; ++it) {
		for (it = low; it != m.end(); ++it) {
			int d = abs(it->first - x.first);
			if (d > x.second) {
				break;
			}
			if (d <= 0) {
				continue;
			}
			d = min(d, it->second);

			II y(it->first, d);
			if (memo.count(y) <= 0) {
				q.push(y);
			}

//			if (M[it->first] < d) {
//				q.push(II(it->first, d));
//			}
		}
	}
	return false;
}



int main(int argc, char *argv[])
{
	cout.precision(16);
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	int t;
	for (t = 0; t < T; ++t) {
		int N, D;
		getline(cin, s);
		{
			stringstream ss(s);
			ss >> N;
		}
		IIVec v(N);
		int i;
		for (i = 0; i < N; ++i) {
			int d, l;
			getline(cin, s);
			stringstream ss(s);
			ss >> d >> l;
			v[i] = II(d, l);
		}
		{
			getline(cin, s);
			stringstream ss(s);
			ss >> D;
		}
		bool result = solve(N, D, v);
		cout << "Case #" << (t+1) << ": " << (result ? "YES" : "NO") << endl;
	}

	return 0;
}

