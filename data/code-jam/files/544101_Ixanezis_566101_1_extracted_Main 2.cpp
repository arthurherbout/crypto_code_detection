#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const long long INF = 0x7f7f7f7f;
long long s[259][259];

long long mabs(long long x) {
	if (x < 0) return -x;
	return x;
}

long long mmax(long long a, long long b) {
	if (a > b) return a;
	return b;
}


int DO(vector<int> x, int I, int m) {
	if (x.size() == 2) {
		int mn = INF;
		for (int i=0; i<=255; i++) {
			for (int u = 0; u<=255; u++) {
				int diff = mabs(x[0] - i) + mabs(x[1] - u);
				if (m == 0) {
					if (i != u) continue;
				} else {
					diff += mmax(0, (mabs(i-u)-1) / m);
				}
				if (diff < mn) {
					mn = diff;
				}
			}
		}
		return mn;
	} else if (x.size() == 3) {
		int mn = INF;
		for (int i=0; i<=255; i++) {
			for (int u = 0; u<=255; u++) {
				for (int t = 0; t<=255; t++) {
					int diff = mabs(x[0] - i) + mabs(x[1] - u) + mabs(x[2] - t);
					if (m == 0) {
						if (!(i == u && u == t)) continue;
					} else {
						diff += mmax(0, (mabs(i-u)-1) / m) * I;
						diff += mmax(0, (mabs(t-u)-1) / m) * I;
					}
					if (diff < mn) {
						mn = diff;
					}
				}
			}
		}
		return mn;
	}
}


int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	//	long long t; cin >> t;

	//for (long long e = 1; e<=t; ++e) {
	//	int n,k;
	//	char s[55][55];
	//	cin >> n >> k;
	//	cin.getline(s[0], 234243);
	//	for (int i=0; i<n; i++) {
	//		cin.getline(s[i], 234243);
	//	}

	//	for (int i=n-1; i>=0; i--) {
	//		int end = n-1;
	//		for (int u=n-1; u>=0; --u) {
	//			if (s[i][u] != '.') {
	//				s[i][end] = s[i][u];
	//				if (u != end)
	//					s[i][u] = '.';
	//				--end;
	//			}
	//		}
	//	}

	//	bool red = false;
	//	bool blue = false;

	//	for (int i=0; i<n; i++) {
	//		for (int u=0; u<n; u++) {
	//			if (s[i][u] != '.') {

	//				// 1
	//				bool can = true;
	//				for (int t=1; t<k; t++) {
	//					if (u+t >= n || s[i][u+t] != s[i][u]) {
	//						can = false;
	//						break;
	//					}
	//				}

	//				if (!can) {
	//					can = true;
	//					for (int t=1; t<k; t++) {
	//						if (i+t >= n || s[i+t][u] != s[i][u]) {
	//							can = false;
	//							break;
	//						}
	//					}

	//					if (!can) {
	//						can = true;
	//						for (int t=1; t<k; t++) {
	//							if (i+t >= n || u+t >=n || s[i+t][u+t] != s[i][u]) {
	//								can = false;
	//								break;
	//							}
	//						}

	//						if (!can) {
	//							can = true;
	//							for (int t=1; t<k; t++) {
	//								if (i-t < 0 || u+t >=n || s[i-t][u+t] != s[i][u]) {
	//									can = false;
	//									break;
	//								}
	//							}
	//						}


	//					}
	//				}

	//				if (can) {
	//					if (s[i][u] == 'R')
	//						red = true;
	//					else blue = true;
	//				}
	//			}
	//		}
	//	}


	//	string ans;
	//	if (red && blue) {
	//		ans = "Both";
	//	} else if (red) {
	//		ans = "Red";
	//	} else if (blue) {
	//		ans = "Blue";
	//	} else ans = "Neither";

	//		

	//	cout << "Case #" << e << ": " << ans << endl;
	//}


	long long t; cin >> t;

	for (long long e = 1; e<=t; ++e) {
		memset(s, 0x7f, sizeof s);
		long long d[266];

		long long D,I, m,n;
		cin >> D>> I >> m >> n;

		if (e == 84) {
			++n;
			--n;
		}

		for (long long i=0; i<n; i++) {
			cin >> d[i];
		}

		//vector<int> x;
		//for (int i=0; i<n; i++) {
		//	x.push_back(d[i]);
		//}

		//long long mn = INF;
		//int res = DO(x, I, m);
		//if (res < mn) {
		//	mn = res;
		//}

		//if (n == 1) {
		//	mn = 0;
		//}


		//if (n == 3) {
		//	for (int i=0; i<n; i++) {
		//		vector<int> y = x;
		//		y.erase(y.begin() + i);
		//		int res = DO(y, I, m) + D;
		//		if (res < mn) {
		//			mn = res;
		//		}
		//	}
		//}
		//	
		//	if (D*2 < mn) {
		//		mn = D*2;
		//	}


		for (long long i=0; i<=256; i++) {
			s[i][0] = mabs(i-d[0]);
			if (n > 1 && D < s[i][0]) {
				s[i][0] = D;
			}
		}

		for (long long u=1; u<n; u++) {

			for (long long i=0; i<=255; i++) {
				long long diff = mabs(d[u] - i);
				for (long long t=0; t<=255; t++) {
					long long g = mabs(i - t);
					long long ins;
					if (m == 0) {
						ins = 100000000;
						if (g <= m) ins = 0;
					} else {
						ins = mmax(0, (g-1) / m);
					}

					if (ins < 10000000 && s[i][u] > s[t][u-1] + diff + ins * I) {
						s[i][u] = s[t][u-1] + diff + ins * I;
					}

				}
				
				if (s[i][u] > s[i][u-1] + D) {
					s[i][u] = s[i][u-1] + D;
				}
			}
		}

		long long mn = INF;

		for (long long i=0; i<=255; i++) {
			if (s[i][n-1] < mn) {
				mn = s[i][n-1];
			}
		}

		cout << "Case #" << e << ": " << mn << endl;
	}

	return 0;
}