//各文字が高々1つらしい（今知ったorz）。
//空行がない場合, 各行独立に決められる。横に伸ばせるだけ伸ばす貪欲とか, 左から適当にやれば簡単に決められる。
//途中に空行がある場合は, 隣り合う行から取ってくる感じでOK。
//最初に「1文字以上ある行」を見つけてくる。そこから上に向かって決める, 下に向かって決める。各行は適当にやる。
#include <iostream>
#include <string>
using namespace std;

int t;
int h, w;
string s[25];

void solve(int row) {
	string ss = s[row];
	int i, j;
	
	for (i = 0; i < ss.length(); ) {
		j = i;
		while (ss[j] == '?' && j < ss.length()) {
			j++;
		}
		if (j >= ss.length()) break;	//ここら辺の条件を見落とさないように！！
		
		//[i, j]をss[j]に書き換える
		for (int k = i; k <= j; k++) {
			ss[k] = ss[j];
		}
		
		//カーソル移動
		i = j + 1;
	}
	
	//最後の???を埋める
	char c;
	for (i = (int)ss.length() - 1; i >= 0; i--) {
		if (ss[i] != '?') { c = ss[i]; break; }
	}
	for (i = 0; i < ss.length(); i++) {
		if (ss[i] == '?') ss[i] = c;
	}
	
	//適当にはい (値コピー)
	s[row] = ss;
}

int main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		int i, j;
		
		cin >> h >> w;
		for (i = 0; i < h; i++) cin >> s[i];
		
		//解く
		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				if (s[i][j] != '?') break;
			}
			if (j < w) break;
		}
		
		int startRow = i;
		for (i = startRow; i >= 0; i--) {
			for (j = 0; j < w; j++) {
				if (s[i][j] != '?') break;
			}
			if (j == w) {	//コピー
				for (j = 0; j < w; j++) s[i][j] = s[i + 1][j];
			}
			else {	//はい
				solve(i);
			}
		}
		for (i = startRow + 1; i < h; i++) {
			for (j = 0; j < w; j++) {
				if (s[i][j] != '?') break;
			}
			if (j == w) {	//コピー
				for (j = 0; j < w; j++) s[i][j] = s[i - 1][j];
			}
			else {	//はい
				solve(i);
			}
		}
		
		//答える
		cout << "Case #" << id << ":" << endl;
		for (i = 0; i < h; i++) cout << s[i] << endl;
	}
	
	return 0;
}