// Google Code Jam 2013 Qualification Round
// Problem D. Treasure

#include <string>
#include <iostream>
#include <sstream>
#include <set>
#include <vector>

using namespace std;

typedef vector<int> IntVec;
typedef set<int> IntSet;


struct node {
	int open;
	char inside[40];
	IntSet queued;
	node();
};
typedef vector<node> NodeVec;


struct route {
	int distance;
	int visited;
	char history[20];
	char keys[40];
	route();
};
typedef vector<route> RouteQueue;


struct solver {
	int K;
	int N;
	NodeVec nodes;
	IntVec solve(route &start);
	IntVec solve();
};


node::node()
{
	memset(inside, 0, sizeof(inside));
}


route::route()
{
	memset(history, 0, sizeof(history));
	memset(keys, 0, sizeof(keys));
}


IntVec solver::solve(route &start)
{
	int i, j, k;
	RouteQueue q;

	for (i = 0; i < N; ++i) {
		if (start.keys[nodes[i].open] > 0) {
			route r;
			r.distance = 1;
			r.visited = 1<<i;
			r.history[0] = i;
			memcpy(r.keys, start.keys, sizeof(start.keys));
			r.keys[nodes[i].open] -= 1;
			q.push_back(r);
			nodes[i].queued.insert(r.visited);
		}
	}

	while (q.size() > 0) {
		RouteQueue next;
		for (int z = 0; z < (int)q.size(); ++z) {
			route &r = q[z];
			int current = r.history[r.distance-1];
			int i;
			for (i = 0; i < 40; ++i) {
				r.keys[i] += nodes[current].inside[i];
			}

			if (r.distance >= N) {
				IntVec v;
				int i;
				for (i = 0; i < N; ++i) {
					v.push_back(r.history[i]);
				}
				return v;
			}

			for (i = 0; i < N; ++i) {
				if (r.visited & (1<<i)) {
					continue;
				}
				if (r.keys[nodes[i].open] <= 0) {
					continue;
				}
				int mask = r.visited | (1<<i);
				IntSet &queued = nodes[i].queued;
				if (queued.find(mask) != queued.end()) {
					continue;
				}
				route n = r;
				n.visited = mask;
				n.history[n.distance++] = i;
				n.keys[nodes[i].open] -= 1;
				next.push_back(n);
				queued.insert(mask);
			}
		}
		q = next;
	}

	// failed
	return IntVec();
}

IntVec solver::solve()
{
	int i, j, k, l;
	string s;
	getline(cin, s);
	{
		stringstream ss(s);
		ss >> K >> N;
	}
	if (K <= 0 || N <= 0) {
		return IntVec();
	}

	route start;
	getline(cin, s);
	{
		stringstream ss(s);
		for (i = 0; i < K; ++i) {
			ss >> k;
			start.keys[k-1] += 1;
		}
	}

	nodes.resize(N);
	for (i = 0; i < N; ++i) {
		getline(cin, s);
		stringstream ss(s);
		ss >> k;
		nodes[i].open = k - 1;
		ss >> k;
		for (j = 0; j < k; ++j) {
			ss >> l;
			nodes[i].inside[l-1] += 1;
		}
	}

	return solve(start);
}

int main(int argc, char *argv[])
{
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	if (T <= 0) return 0;

	for (int t = 0; t < T; ++t) {
		cout << "Case #" << (t+1) << ":";
		solver x;
		IntVec ans = x.solve();
		if (ans.size() <= 0) {
			cout << " IMPOSSIBLE";
		} else {
			for (int i = 0; i < (int)ans.size(); ++i) {
				cout << " " << (ans[i] + 1);
			}
		}
		cout << endl;
	}

    return 0;
}

