//長さLの文字列が与えられるので、これをX回繰り返した文字列において、ijkという並びを作ることができるかを判定せよ。
//なお、変換表(2つ→1つ)は与えらえる(これは4元数に基づく)
//愚直解(結合則より、左から計算しても良いし、どこから計算しても良い), ただし区切る場所を変えたら違う3文字が出てくることがあるし、
//ijk = 1だがkji = 1も成り立つので「1」にまとめるのはNG。ijkと=で結ばれるものでも最終的にijkにはできない場合があるので注意。
//ただし、(-i)(-j)k = ijkとはできるので、(-i)(-j)kもOKである。要するに、1文字→2文字の変換を禁止したうえで「ijk」を出現させる感じのやつだ。
//すなわち「jkkik」は「(-i)(-j)k」→「ijk」のように変形できるのでYES.
#include<iostream>
#include<string>
#include<algorithm>
#include<functional>
#include<vector>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

void Write(char *name, int T);

int t;
char out[100][1002];

//変換表(符号なし. 1は簡単のためhとしている)
char *conv[4] = {"hijk", "ihkj", "jkhi", "kjih"};
//符号スワップの有無
int issign[4][4] = {{0,0,0,0},{0,1,0,1},{0,1,1,0},{0,0,1,1}};

//O((L*X)^2)
string solve(string S) {
	int i, j, k;
	int slen = S.length();
	
	static char C[10011][10011];
	static bool Sign[10011][10011] = {false};
	
	for( i = 0; i < slen; i++ ) {
		char c = S[i];
		int minus = 0;
		
		C[i][i] = c;
		Sign[i][i] = false;
		for( j = i+1; j < slen; j++ ) {
			int u = c - 'h';
			int v = S[j] - 'h';
			c = conv[u][v];
			minus += issign[u][v];
			
			C[i][j] = c;
			Sign[i][j] = ((minus%2) == 1);
		}
	}
	
	for( i = 0; i < slen - 2; i++ ) {
		for( j = i+1; j < slen - 1; j++ ) {
			//cout << "i = " << i << " j = " << j << endl;
			//cout << C[0][i] << C[i+1][j] << C[j+1][slen-1] << endl << endl;
			
			//(1, i], (i+2, j], (j+2, S.length() - 1]のそれぞれがi, j, kとなるかを調査
			if ( C[0][i] != 'i' )
				break;
			
			if ( C[i+1][j] != 'j' )
				continue;
				
			if ( C[j+1][slen-1] != 'k' )
				continue;
			
			int Minus = (Sign[0][i]==true) + (Sign[i+1][j]==true) + (Sign[j+1][slen-1]==true);
			if ( Minus % 2 == 0 )
				return "YES";
		}
	}
	return "NO";
}

signed main() {
	int i, j;
	cin >> t;
	
	for( i = 0; i < t; i++ ) {
		int l, x;
		string s;
		cin >> l >> x;
		cin >> s;
		
		string S;
		for( j = 0; j < x; j++ )
			S += s;
		
		string res = solve(S);
		sprintf(out[i], "Case #%d: %s\n", i+1, res.c_str() );
	}
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\qual\\out-c-small.txt", t);
	return 0;
}

//outの中を出力(改行なし)
void Write(char *name, int T) {
	FILE *fp = fopen(name, "w");
	for(int i = 0; i < T; i++ ) {
		fprintf(fp, "%s", out[i]);
		fprintf(stdout, "%s", out[i]);
	}
	fclose(fp);
}