//問題
//R*Cのボードに1*Wの船がある。船はボードの外枠とくっついている（誤読、ボードからはみ出さないだけ）。
//船が与えられた場所を自分は知らない。船の占有するマスを全て特定するために
//何個のマスを調べるべきか。最小化し、その最大値を答えよ。
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
#define int long long
using namespace std;

void Read(char *name );
void Write(char *name, int T);

int t;
char out[100][1002];
int r[100], c[100], w[100];

int digit2(int x)
{
	int i;
	for ( i = 30; (x>>i) == 0 && i >= 0; i-- );
	return i;
}
int solve(int row, int col, int wide)
{
	double eps = 1e-7;
	return digit2(col - wide) + 1 + wide;
}

signed main() {
	int i, j;
	Read("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1c\\in-a-small.txt");
	
	for( i = 0; i < t; i++ ) {
		int ans = solve(r[i], c[i], w[i]);
		cout << i << " : OK" << endl;
		sprintf(out[i], "Case #%lld: %lld\n", i+1, ans);
	}
	
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1c\\out-a-small.txt", t);
	return 0;
}

//ファイル入力(コンソールからだと応答なしになる(Alargeでの教訓) )
void Read(char *name ) {
	FILE *fp = fopen(name, "r");
	if ( fp == NULL ) {
		printf("%s is not found.\n", name);
		return;
	}
	
	fscanf(fp, "%lld", &t);
	for(int i = 0; i < t; i++ ) {
		fscanf(fp, "%lld%lld%lld", r+i, c+i, w+i);
	}
	fclose(fp);
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