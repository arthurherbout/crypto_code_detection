#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <utility>
#include <cmath>

const double PI = 3.141592653589793238463;

typedef unsigned long long ull;

using namespace std;

int planningC[1440];
int planningJ[1440];
int mc[721][1441];
int mj[721][1441];


void SolveA(vector<pair<double, double>>& P, ull K) {
	double ans = 0;
	for (int i = 0; i < P.size(); ++i) {
		double x = P[i].second;
		x += P[i].first;
		int k = 1;
		for (int j = P.size() - 1; j >= 0; --j) {
			if (k == K) {
				break;
			}
			if (j == i) {
				continue;
			}
			if (P[j].second > P[i].second) {
				continue;
			}
			++k;
			x += P[j].first;
		}
		if (k == K) {
			ans = max(ans, x);
		}
	}
	cout.precision(10);
	cout << PI * ans;
}


int GetMinSwitchToC(int c, int t, bool StartWithC);


int GetMinSwitchToJ(int c, int t, bool StartWithC) {
	if (c < 0 || c > t || planningJ[t] == 0) {
		return 200;
	}

	if (mj[c][t] != -1) {
		return mj[c][t];
	}

	if (t == 0) {
		mj[c][t] = StartWithC ? 1 : 0;
		return mj[c][t];
	}

	int ans = 200;
	if (planningJ[t - 1]) {
		ans = min(ans, GetMinSwitchToJ(c, t - 1, StartWithC));
	}
	if (planningC[t - 1]) {
		ans = min(ans, GetMinSwitchToC(c - 1, t - 1, StartWithC) + 1);
	}
	mj[c][t] = ans;
	return ans;
}


int GetMinSwitchToC(int c, int t, bool StartWithC) {
	if (c < 0 || c > t || planningC[t] == 0) {
		return 200;
	}
	if (mc[c][t] != -1) {
		return mc[c][t];
	}

	if (t == 0) {
		mc[c][t] = StartWithC ? 0 : 1;
		return mc[c][t];
	}

	int ans = 200;
	if (planningC[t - 1] > 0) {
		ans = min(ans, GetMinSwitchToC(c - 1, t - 1, StartWithC));
	}
	if (planningJ[t - 1] > 0) {
		ans = min(ans, GetMinSwitchToJ(c, t - 1, StartWithC) + 1);
	}
	mc[c][t] = ans;
	return ans;
}

void ResetMemory(vector<pair<int, int>>& C, vector<pair<int, int>>& J) {
	for (int c = 0; c < 721; ++c) {
		for(int t = 0; t < 1441; ++t) {
			mc[c][t] = -1;
			mj[c][t] = -1;
		}
	}

	for (int t = 0; t < 1440; ++t) {
		planningC[t] = 1;
		planningJ[t] = 1;
	}

	for (auto p : C) {
		for (int i = p.first; i < p.second; ++i) {
			planningC[i] = 0;
		}
	}

	for (auto p : J) {
		for (int i = p.first; i < p.second; ++i) {
			planningJ[i] = 0;
		}
	}
}

void SolveB(vector<pair<int, int>>& C, vector<pair<int, int>>& J) {

	ResetMemory(C, J);
	//for (int t = 0; t <= 1440; ++t) {
	//	for(int c =0; c<=min(t,720); ++c)
	//		GetMinSwitchToC(c, t, true);
	//}
	int a = GetMinSwitchToC(720, 1440, true);

	ResetMemory(C, J);
	/*for (int t = 0; t <= 1440; ++t) {
		for (int c = 0; c <= min(t, 720); ++c)
			GetMinSwitchToJ(c, t, false);
	}*/
	int b = GetMinSwitchToJ(720, 1440, false);
	cout << min(a, b);
}

void main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; ++i) {
		int Ac, Aj;
		cin >> Ac >> Aj;
		vector<pair<int, int>> C;
		vector<pair<int, int>> J;
		for (int j = 0; j < Ac; ++j) {
			int s, e;
			cin >> s >> e;
			C.push_back(make_pair(s, e));
		}
		for (int j = 0; j < Aj; ++j) {
			int s, e;
			cin >> s >> e;
			J.push_back(make_pair(s, e));
		}
		sort(C.begin(), C.end());
		sort(J.begin(), J.end());
		cout << "Case #" << i + 1 << ": ";
		SolveB(C,J);
		cout << endl;
	}
}