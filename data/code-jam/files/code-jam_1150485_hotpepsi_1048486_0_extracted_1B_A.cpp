/*

Google Code Jam 2011 Round 1B

Problem A. RPI

*/


#include <stdio.h>
#include <string.h>
#include <list>
#include <vector>
#include <map>
#include <set>

using namespace std;

typedef __int64 __ll;

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
		int teams;
		if (!getInt(teams) || teams < 1) {
			return false;
		}

		int matrix[128][128] = {0};
		int i, j;
		for (i = 0; i < teams; ++i) {
			char *p = getSingleLine();
			if (!p) return false;
			if ((int)strlen(p) < teams) return false;

			for (j = 0; j < teams; ++j) {
				matrix[i][j] = 0;
				if (*p == '0') {
					matrix[i][j] = -1;
				}
				if (*p == '1') {
					matrix[i][j] = 1;
				}
				++p;
			}
		}

		int games[128] = {0};
		int wins[128] = {0};
		for (i = 0; i < teams; ++i) {
			int g = 0;
			int w = 0;
			for (j = 0; j < teams; ++j) {
				if (matrix[i][j]) {
					++g;
					if (matrix[i][j] > 0) {
						++w;
					}
				}
			}
			games[i] = g;
			wins[i] = w;
		}

		double wp[128] = {0};
		double owp[128] = {0};
		double oowp[128] = {0};

		for (i = 0; i < teams; ++i) {
			// calc wp
			if (games[i] > 0) {
				wp[i] = (double)(wins[i]) / (double)(games[i]);
			}

			// calc owp
			int owpcnt = 0;
			double owpsum = 0;
			double tmp[128] = {0};
			for (j = 0; j < teams; ++j) {
				int wl = matrix[i][j];
				if (i == j || !wl) {
					continue;
				}
				int g = (games[j] - 1);
				int w = wins[j];
				if (wl < 0) {
					w -= 1;
				}
				if (g > 0) {
					owpcnt += 1;
					owpsum += (double)w / (double)g;
				}
			}
			if (owpcnt > 0) {
				owp[i] = owpsum / (double)owpcnt;
			}
		}

		// calc oowp
		for (i = 0; i < teams; ++i) {
			if (games[i] > 0) {
				double sum = 0;
				int cnt = 0;
				for (j = 0; j < teams; ++j) {
					if (matrix[i][j]) {
						sum += owp[j];
						cnt += 1;
					}
				}
				if (cnt > 0) {
					oowp[i] = sum / (double)cnt;
				}
			}
		}

		string result;

		printf("Case #%d:\n", case_index + 1);
		for (i = 0; i < teams; ++i) {
//			printf("%.12f\n", wp[i]);
//			printf("%.12f\n", owp[i]);
//			printf("%.12f\n", oowp[i]);
			double rpi = 0.25 * wp[i] + 0.5 * owp[i] + 0.25 * oowp[i];
			printf("%.12f\n", rpi);
		}
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

