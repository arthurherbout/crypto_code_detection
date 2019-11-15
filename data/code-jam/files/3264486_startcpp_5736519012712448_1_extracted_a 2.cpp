//DoArray[i] == left if 「i回目に「パンケーキleft～left+K-1をひっくり返す」という操作列を考える。0-indexed。
//例えば, サンプル1の説明ならDoArray = {0, 5, 4}になる。
//操作順は解に影響しないことから、DoArrayをソートしてもよい。
//たとえば, サンプル1で, DoArray = {0, 4, 5}とした場合も, 
//[---]+-++- -> ++++[-++]- -> +++++[---] -> ++++++++ とパンケーキをすべて幸せにできる。
//よってDoArrayは昇順, としてよい。
//するとDoArray = {…, x, x, …} (xは任意の整数)みたいなのは無意味（同じ箇所で2回ひっくり返しても無駄）なことから,
//操作(DoArray)が一意に定まる。実装時には, 「パンケーキiを左端にひっくり返すか？」を各i = 0, 1, …について求めると楽。
//あとは, 実際にすべて+になったかを判定すればよい。

#include <iostream>
#include <string>
using namespace std;

int t;
string s;
int k;

int solve() {
	int i, j;
	int flipCnt = 0;
	
	for (i = 0; i < (int)s.length() - k + 1; i++) {
		if (s[i] == '-') {
			for (j = 0; j < k; j++) {
				if (s[i + j] == '-') s[i + j] = '+';
				else s[i + j] = '-';
			}
			flipCnt++;
		}
	}
	
	for (i = 0; i < s.length(); i++) {
		if (s[i] == '-') {
			return -1;
		}
	}
	return flipCnt;
}

int main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		cin >> s >> k;
		int res = solve();
		
		cout << "Case #" << id << ": ";
		if (res == -1) { cout << "IMPOSSIBLE"; }
		else { cout << res; }
		cout << endl;
	}
	return 0;
}