#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

const int N = 110;
const int IT = 12000;
vector <int> p[N];
int fa[N], cnt[N];
char s[10][30], fst[N];

string comb(char ch, vector <string> &res) {
	static int idx[N], tmp[N];
	char buf[N];
	int tot = 0, ptr = 0;
	for (unsigned i = 0; i < res.size(); i++) {
		int l = res[i].length();
		for (int j = 0; j < l; j++)
			tmp[ptr++] = i;
	}
	tot = ptr;
	for (int i = 0; i < tot; i++) {
		int k = rand() % (tot - i);
		swap(tmp[i], tmp[i + k]);
	}
	memset(idx, 0, sizeof(int) * res.size());
	buf[0] = ch;
	ptr = 1;
	for (int i = 0; i < tot; i++) {
		int id = tmp[i];
		buf[ptr++] = res[id][idx[id]];
		idx[id]++;
	}
	buf[ptr] = 0;
	return string(buf);
}

string sample(int k) {
	if (p[k].size() == 0) {
		char buf[2] = {fst[k], 0};
		return string(buf);
	}
	vector <string> res;
	for (unsigned i = 0; i < p[k].size(); i++)
		res.push_back(sample(p[k][i]));
	return comb(fst[k], res);
}

void run(int cas) {
	int n, m;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d", fa + i);
	for (int i = 0; i <= n; i++)
		p[i].clear();
	for (int i = 1; i <= n; i++)
		p[fa[i]].push_back(i);
	fst[0] = ' ';
	scanf(" %s", fst + 1);
	scanf("%d", &m);
	memset(cnt, 0, sizeof(cnt));
	for (int i = 0; i < m; i++)
		scanf(" %s", s[i]);
	for (int it = 0; it < IT; it++) {
		string str = sample(0);
		for (int i = 0; i < m; i++)
			if (strstr(str.c_str(), s[i]) != NULL)
				cnt[i]++;
	}
	printf("Case #%d:", cas);
	for (int i = 0; i < m; i++)
		printf(" %f", cnt[i] * 1.0 / IT);
	printf("\n");
	fflush(stdout);
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}

