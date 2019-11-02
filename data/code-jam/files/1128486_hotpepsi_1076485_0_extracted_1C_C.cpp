/*

Google Code Jam 2011 Round 1C

Problem C. Perfect Harmony

*/


#include <stdio.h>
#include <string.h>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

typedef __int64 __ll;
typedef vector<int> VI;
typedef set<int> SI;

class GCJ {
	static const size_t BufferSize = 1024000;
	FILE *m_Source;
	size_t m_BufferSize;
	char *m_Buffer;

public:
	GCJ(FILE *source, size_t buffer_size = BufferSize) {
		m_Source = source;
		m_BufferSize = buffer_size;
		m_Buffer = new char[m_BufferSize];
	}

	~GCJ() {
		delete [] m_Buffer;
	}

	bool solve(void) {
		int cases;
		if (!getInt(cases) || cases < 1) {
			return false;
		}

		int solved = 0;
		int i;
		for (i = 0; i < cases; ++i) {
			if (solve(i)) {
				++solved;
			}
		}
		return solved >= cases;
	}

private:

	bool solve(int case_index) {
		int N, L, H;
		char *p = getSingleLine();
		if (!p) return false;
		if (sscanf(p, "%d %d %d", &N, &L, &H) != 3) {
			return false;
		}

		int i, j;
		int x, y;

		VI _v;

		p = getSingleLine();
		if (!p) return false;

		SI gcds;

		bool result = true;

		p = strtok(p, " ,");
		for (i = 0; i < N; ++i) {
			if (!p) return false;
			_v.push_back(atoi(p));
			gcds.insert(atoi(p));
			p = strtok(NULL, " ,");
		}

		sort(_v.begin(), _v.end());

/*
		for (i = 0; i < _v.size(); ++i) {
			bool ok = false;
			for (j = 0; j < _v.size(); ++j) {
				if (i != j) {
					if ((_v[i] % _v[j]) == 0 || (_v[j] % _v[i]) == 0) {
						ok = true;
					}
				}
			}
			if (!ok) {
				printf("Case #%d: NO\n", case_index + 1);
				return true;
			}
		}
*/

		for (x = L; x <= H; ++x) {
//			if (gcds.find(x) != gcds.end()) {
//				continue;
//			}
			bool ok = true;
			for (i = 0; i < _v.size(); ++i) {
				int y = _v[i];
				if ((x % y) != 0 && (y % x) != 0) {
					ok = false;
					break;
				}
			}
			if (ok) {
				printf("Case #%d: %d\n", case_index + 1, x);
				return true;
			}
		}

		printf("Case #%d: NO\n", case_index + 1);
		return true;
	}

	int getCases(void) {
		int cases;
		char *p = getSingleLine();
		if (p != NULL && sscanf(p, "%d", &cases) >= 1) {
			return cases;
		}
		return -1;
	}

	bool getInt(int &i) {
		char *p = getSingleLine();
		return p != NULL && sscanf(p, "%d", &i) >= 1;
	}

	char *getSingleLine(void) {
		char *p = fgets(m_Buffer, m_BufferSize, m_Source);
		if (!p) {
			return NULL;
		}
		char *term = p + strlen(p) - 1;
		if (term > p && *term == '\n') {
			*term = 0;
		}
		return p;
	}

};


int main()
{
	GCJ gcj(stdin);
	return gcj.solve() ? 0 : -1;
}

