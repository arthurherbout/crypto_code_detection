/*

Qualification Round 2011
Problem D. GoroSort


Problem

Goro has 4 arms. Goro is very strong. You don't mess with Goro.
Goro needs to sort an array of N different integers.
Algorithms are not Goro's strength; strength is Goro's strength.
Goro's plan is to use the fingers on two of his hands
to hold down several elements of the array and hit the table
with his third and fourth fists as hard as possible.
This will make the unsecured elements of the array fly up into the air,
get shuffled randomly, and fall back down into the empty array locations.

Goro wants to sort the array as quickly as possible.
How many hits will it take Goro to sort the given array, on average,
if he acts intelligently when choosing which elements of the array
to hold down before each hit of the table?

More precisely, before each hit, Goro may choose any subset of
the elements of the array to freeze in place. He may choose differently
depending on the outcomes of previous hits. Each hit permutes the
unfrozen elements uniformly at random. Each permutation is equally likely.


Input

The first line of the input gives the number of test cases, T.
T test cases follow. Each one will consist of two lines.
The first line will give the number N. The second line will list
the N elements of the array in their initial order.


Output

For each test case, output one line containing "Case #x: y",
where x is the case number (starting from 1) and y is the expected number
of hit-the-table operations when following the best hold-down strategy.
Answers with an absolute or relative error of at most 10-6 will be
considered correct.


Limits

1 <= T <= 100;
The second line of each test case will contain a permutation of the N
smallest positive integers.
Goro has more than N fingers on each hand.

Small dataset

1 <= N <= 10;

Large dataset

1 <= N <= 1000;


Sample

Input
 
3
2
2 1
3
1 3 2
4
2 1 4 3


Output

Case #1: 2.000000
Case #2: 2.000000
Case #3: 4.000000

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
typedef list<int> LI;
typedef vector<int> VI;
typedef map<int, int> MI;

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
		int N;
		if (!getInt(N)) {
			return false;
		}

		char *p = getSingleLine();
		if (!p) return false;
		const char *delim = "\t ";
		p = strtok(p, delim);
		if (!p) return false;
		VI seq;
//		seq.reserve(1000);
		while (p) {
			int n = atoi(p);
			if (n < 1) return false;
			seq.push_back(n);
			p = strtok(NULL, delim);
		}

		double sum = 0;
		int i;
		for (i = 0; i < seq.size(); ++i) {
			int v = seq[i];
			if (v == (i + 1)) {
				continue;
			}
			sum += 1;
			if (seq[v - 1] == (i + 1)) {
				continue;
			}
			sum += 1;
		}

		printf("Case #%d: %.6f\n", case_index + 1, sum);
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

