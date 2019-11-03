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
int b[100], n[100];
int m[100][1000];

char out[100][1002];

int solve1(int tnum) {
	//愚直O(n[tnum]logb[tnum])
	//優先度(次に切る時間→番号)
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > que;
	
	int B = b[tnum];
	int N = n[tnum], i;
	
	for( i = 0; i < B; i++ ) {
		que.push(pair<int, int>(0, i+1) );
	}
	
	for( i = 0; i < N-1; i++ ) {
		pair<int, int> now = que.top();
		que.pop();
		que.push(pair<int, int>(now.first + m[tnum][now.second-1], now.second) );
	}
	return (que.top().second);
}

//Xの長さは、O(M(MIN)/B * N)くらい必要になるので、smallも解けない。
int solve2(int tnum) {
	static int X[1000001];	//i分目開始時にカットが終わった客の数をX[i]とする。
	int i, j;
	int B = b[tnum];
	int N = n[tnum];
	
	for( i = 0; i < 1000001; i++ )
		X[i] = 0;

	for( i = 0; i < B; i++ ) {
		for( j = 1; j < 1000000; j += m[tnum][i] ) {
			X[j]++;
		}
	}
	for( j = 1; j <= 1000000; j++ )
		X[j] += X[j-1];
	
	int *it = lower_bound(X, X+1000000, N);
	
	int time = it - X - 1;
	int num = N - X[time];
	
	cout << time << " " << num << endl;
	//time分目にカットを始められる人の内、num番目に番号の小さい人を探す(ただし1番目に小さい is 最小とする)
	for( i = 0; i < B; i++ ) {
		if ( time % m[tnum][i] == 0 )
			num--;
		if ( num == 0 )
			break;
	}
	return i+1;
}

//x分目終了時に何人の人の紙が切られたかを返す
int cuted(int tnum, int x) {
	int i;
	int ret = 0;
	if ( x < 0 )
		return 0;
	for( i = 0; i < b[tnum]; i++ )
		ret += x/m[tnum][i] + 1;
	return ret;
}

//timeが求まれば楽勝だと分かった。
//各人に注目すると、人iはtime分までにtime/m[i]人を散髪している。
int solve3(int tnum) {
	int st = 0, ed = 10000000000010, medi;
	int res;
	
	//for(int i = 0; i < 30; i++ )
	//	cout << cuted(tnum, i) << endl;
	
	//cuted(tnum, x) >= n[tnum]となる最小のxを探す
	while(st <= ed) {
		medi = st+ed;
		medi /= 2;
		res = cuted(tnum, medi);
		
		if ( res < n[tnum] ) {
			st = medi+1;
		}
		else {
			if ( cuted(tnum, medi-1) < n[tnum] )
				break;
			ed = medi-1;
		}
	}
	int time = medi;
	int num = n[tnum] - cuted(tnum, time-1);
	
	//cout << time << " " << num << endl;
	
	int i;
	//time分目にカットを始められる人の内、num番目に番号の小さい人を探す(ただし1番目に小さい is 最小とする)
	for( i = 0; i < b[tnum]; i++ ) {
		if ( time % m[tnum][i] == 0 )
			num--;
		if ( num == 0 )
			break;
	}
	return i + 1;
}

signed main() {
	int i, j;
	Read("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1\\in-b-large.txt");
	
	for( i = 0; i < t; i++ ) {
		int ans = solve3(i);
		cout << i << " : OK" << endl;
		sprintf(out[i], "Case #%lld: %lld\n", i+1, ans);
	}
	
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1\\out-b-large.txt", t);
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
		fscanf(fp, "%lld%lld", &b[i], &n[i]);
		for(int j = 0; j < b[i]; j++ )
			fscanf(fp, "%lld", &m[i][j]);
	}
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