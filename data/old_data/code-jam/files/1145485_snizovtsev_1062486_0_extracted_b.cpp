#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

#define foreach(iter, cont) \
	for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)

struct Word {
	char* str;
	int mask[26];
	int len;
	Word(char* _str) {
		str = _str;
		len = strlen(str);
		memset(mask, 0, sizeof(mask));
		for (int i = 0; i < len; ++i) {
			for (int a = 'a'; a <= 'z'; ++a) {
				mask[a-'a'] = (mask[a-'a'] << 1) + (str[i] == a);
			}
		}
	}
};

void solve ()
{
	int n, m;
	scanf("%d %d", &n, &m);
	char sdata[n][20];
	Word* wdata[n];
	list <Word*> dict[20];
	for (int i = 0; i < n; ++i) {
		scanf("%s", sdata[i]);
		wdata[i] = new Word(&sdata[i][0]);
		dict[wdata[i]->len].push_back(wdata[i]);
	}

	for (int q = 0; q < m; ++q) {
		char query[27];
		scanf("%s", query);
		char* best; int val = -1;

		for (int x = 0; x < n; ++x) {
			Word* orig = wdata[x];
			list <Word*> p = dict[orig->len];
			bool word_set[27];
			memset(word_set, 0, sizeof(word_set));
			for (int i = 0; i < orig->len; ++i)
				word_set[orig->str[i]-'a'] = true;

			int points = 0;
			for (int a = 0; a < 26; ++a) {
				bool del = false;
				list <Word*> newp;
				foreach(it, p) {
					Word* w = *it;
					if (w->mask[query[a]-'a'] != orig->mask[query[a]-'a'])
						del = true;
					else
						newp.push_back(*it);
//					printf("%s ", w->str);
				}
//				printf("\n");
				p = newp;

				if (del && !word_set[query[a]-'a']) ++points;
				//if (del) ++points;
				//if ((!del) && word_set[query[a]-'a']) ++points;
				//fprintf(stderr, "%d %c %d\n", x+1, query[a], points);
			}

			if (points > val) {
				val = points;
				best = orig->str;
			}
		}
		printf("%s ", best);
	}
	printf("\n");

	for (int i = 0; i < n; ++i)
		delete wdata[i];
}

int main ()
{
	int T;
	scanf ("%d", &T);
	for (int i = 1; i <= T; i++) {
		printf ("Case #%d: ", i);
		solve();
	}
	return 0;
}
