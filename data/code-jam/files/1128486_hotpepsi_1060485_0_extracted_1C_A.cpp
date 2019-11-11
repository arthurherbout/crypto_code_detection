/*

Google Code Jam 2011 Round 1C

Problem A. 

*/


#include <stdio.h>
#include <string.h>
#include <list>
#include <vector>
#include <map>
#include <set>

using namespace std;

typedef __int64 __ll;
typedef vector<string> StringVector;

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
		int R, C;
		char *p = getSingleLine();
		if (!p) return false;
		if (sscanf(p, "%d %d", &R, &C) != 2) {
			return false;
		}

		StringVector lines;

		char buffer[64][64] = {0};
		int i, j;
		for (i = 0; i < R; ++i) {
			char *p = getSingleLine();
			if (!p) return false;
			strcpy(buffer[i], p);
		}

		bool OK = true;
		for (i = 0; OK && i < R; ++i) {
			for (j = 0; OK && j < C; ++j) {
				if (buffer[i][j] != '#') {
					continue;
				}
				if (buffer[i][j + 1] != '#' || buffer[i + 1][j] != '#' || buffer[i + 1][j + 1] != '#') {
					OK = false;
					break;
				}
				buffer[i][j] = '/';
				buffer[i][j + 1] = '\\';
				buffer[i + 1][j] = '\\';
				buffer[i + 1][j + 1] = '/';
				++j;
			}
		}

		printf("Case #%d:\n", case_index + 1);
		if (OK) {
			for (i = 0; i < R; ++i) {
				printf("%s\n", buffer[i]);
			}
		} else {
			printf("Impossible\n");
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

