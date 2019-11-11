// Google Code Jam 2013 R1B
// Problem A. 

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long LL;

typedef vector<LL> LLVec;

LL solve(const string &name, int L)
{
	int length = (int)name.length();
	const char *start = name.c_str();
	const char *end = start + length;
	static int l[7000000];
	static int f[7000000];
	static int pos[7000000];
	memset(pos, -1, sizeof(pos));
	int v[256] = {};
	v['a'] = 1;
	v['e'] = 1;
	v['i'] = 1;
	v['o'] = 1;
	v['u'] = 1;
	int i, j, k;
	int c = 0;
	int w = 0;
	for (i = 0; i < length; ++i) {
		if (v[start[i]]) {
			continue;
		}
		for (j = i; j < length; ++j) {
			if (v[start[j]]) {
				break;
			}
		}
		int x = j - i;
		if (x >= L) {
			pos[w] = i;
			f[w++] = x;
		}
		i = j;
	}

	if (w <= 0) {
		return 0;
	}

	LL ans = 0;
	k = 0;
	for (i = 0; i < length; ++i) {
		int s;
		if (pos[k] > i) {
			ans += (length - (pos[k] + L) + 1);
			continue;
		}
		if ((f[k] - (i - pos[k])) >= L) {
			s = i + L;
		} else {
			++k;
			if (pos[k] < 0) {
				break;
			}
			s = pos[k] + L;
		}
		ans += length - s + 1;
	}

	return ans;
}

int main(int argc, char *argv[])
{
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	if (T <= 0) return 0;

	for (int t = 0; t < T; ++t) {
		int L = 0;
		getline(cin, s);
		stringstream ss(s);
		string name;
		ss >> name >> L;
		cout << "Case #" << (t+1) << ": " << solve(name, L) << endl;
	}

	return 0;
}

