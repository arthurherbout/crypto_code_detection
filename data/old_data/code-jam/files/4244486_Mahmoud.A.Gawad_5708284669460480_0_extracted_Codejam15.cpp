//============================================================================
// Name        : Codejam15.cpp
// Author      : Mahmoud Saleh A. Gawad
// Version     :
// Copyright   : None.
// Description : Codejam'15 Qualification Round.
//============================================================================

#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;

#define pcase(tt)  printf("Case #%d: ", tt)
#define mem(arr,val) memset(arr,val,sizeof arr)
#define loopi(i,val,n) for(i=val;i<=n;i++)
#define loopd(i,val,val2) for(i=val;i>=val2;i--)
#define sz(vec) (int)vec.size()
#define print1(n) out << n <<endl
#define print2(n,m) out << n<<" "<<m<<endl
#define print3(n,m,l) out << n<<" "<<m<< " "<<l<<endl

const char* fin[] = { "a.txt", "b.txt", "c.txt", "d.txt" };
const char* fout[] = { "a.out", "b.out", "c.out", "d.out" };
const int IDX = 1;

int K, L, S, len, Max;
string target;
char arr[7];
int cnt[8];
void check(string s) {
	int res = 0;
	for (int i = 0; i < S - L + 1; i++) {
		bool flag = true;
		for (int j = 0; j < L; j++) {
			if (s[i + j] != target[j]) {
				flag = false;
				break;
			}
		}
		if (flag) {
			res++;
		}
	}
	Max = max(res, Max);
	cnt[res]++;
}

void solve(int i, string s) {
	if (i == S) {
		check(s);
		return;
	}
	for (int j = 0; j < len; j++) {
		solve(i + 1, s + arr[j]);
	}
}

int main() {
	ifstream in(fin[IDX]);
	freopen(fout[IDX],"w",stdout);

	int TC;
	in >> TC;
	for (int tt = 1; tt <= TC; tt++) {
		in >> K >> L >> S;
		string board;
		in >> board;
		in >> target;
		int a[26];
		mem(a, 0);
		len = 0;
		for (int i = 0; i < (int) board.size(); i++) {
			arr[len++] = board[i];
			a[board[i] - 'A'] = 1;
		}
		Max = 0;
		mem(cnt, 0);
		solve(0, "");
		int power = 1;
		for (int i = 0; i < S; i++)
			power *= len;
		int sum = 0;
		for (int i = 1; i <= Max; i++) {
			sum += i*cnt[i];
		}
		pcase(tt);
		printf("%.7f\n",(1. * Max - 1. * sum / power) );
	}
	return 0;
}

