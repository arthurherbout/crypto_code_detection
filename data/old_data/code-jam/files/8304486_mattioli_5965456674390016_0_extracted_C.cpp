#include <bits/stdc++.h>

#define debug(x) cout << #x << " = " << x << endl
#define fori(i, ini, lim) for(int i = int(ini); i < int(lim); i++)
#define ford(i, ini, lim) for(int i = int(ini); i >= int(lim); i--)

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;

const int MAX = 1e3 + 5;
int C;
int E[MAX][2], L[MAX][2], D[MAX][2], A[MAX][2], A_cnt[MAX];

int eval(int a, int l) {
	if(l >= a) {
		return l - a;
	}
	return ((l + 24) - a);
}

int main() {
	ios_base::sync_with_stdio(false);

	int t;
	cin >> t;
	int kase = 1;
	while(t--) {
		memset(A_cnt, 0, sizeof A_cnt);
		cin >> C;
		int ans = 0;
		fori(i, 1, C + 1) {
			cin >> E[i][0] >> L[i][0] >> D[i][0];
			cin >> E[i][1] >> L[i][1] >> D[i][1];
			A[E[i][0]][A_cnt[E[i][0]]++] = (L[i][0] + D[i][0]) % 24;
			A[E[i][1]][A_cnt[E[i][1]]++] = (L[i][1] + D[i][1]) % 24;
			ans += D[i][0];
			ans += D[i][1];
		}
		fori(i, 2, C + 1) {
			int cur = min(eval(A[i][0], L[i][0]) + eval(A[i][1], L[i][1]), eval(A[i][0], L[i][1]) + eval(A[i][1], L[i][0]));
			ans += cur;
		}
		int aux = eval(0, L[1][0]) + min(eval(A[1][0], L[1][1]), eval(A[1][1], L[1][1]));
		int aux2 = eval(0, L[1][1]) + min(eval(A[1][0], L[1][0]), eval(A[1][1], L[1][0]));
		ans += min(aux, aux2);
		cout << "Case #" << kase++ << ": " << ans << '\n';
	}

	return 0;
}

