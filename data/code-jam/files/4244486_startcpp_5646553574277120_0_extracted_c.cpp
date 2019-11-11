//���F
//D��ނ̍d��[�~]���^������B�e�d�݂�0�`C���g����(�ǉ������e�d�݂ɂ��Ă�0�`C���g����)�B
//1�`V[�~]��S�ĕ\���������B
//�������邽�߂ɁA�ǉ����ׂ��d�݂̎�ސ����ŏ�������B
//���Ȃ݂ɁA������ނ̃R�C����2��ȏ�ǉ����āA0�`2C���g����悤�ɂ���Ȃǂ́A���R�ł��Ȃ����̂Ƃ���B(�v����ɓ�����ނ̍d�݂𓱓����邱�Ƃ͂ł��Ȃ�)
//Small : C = 1, 1 <= D <= 5, 1 <= V <= 30 (�d�݂̉��lWi <= V, Wi�͑��قȂ�)
//Large : 1 <= C <= 100, 1 <= D <= 100, 1 <= V <= 10^9 (�d�݂̉��lWi <= V, Wi�͑��قȂ�)
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
int c[100], d[100], v[100];
int w[100][110];

int ans, wlen;
bool used[101];
bool DP(int ban) {
	bool dp[35][31] = {false};
	int i, j, k;
	dp[0][0] = true;
	//for (i = 0; i < wlen; i++)
	//	cout << w[ban][i] << " ";
	//cout << endl;
	
	for (i = 0; i < wlen; i++) {
		for (j = 0; j <= v[ban]; j++) {
			//cout << dp[i][j] << " ";
			if (dp[i][j] == false)
				continue;
			for (k = 0; k <= c[ban]; k++) {
				if (j + k * w[ban][i] > v[ban])
					break;
				dp[i+1][j + k * w[ban][i]] = true;
			}
		}
		//cout << endl;
	}
	for (i = 0; i <= v[ban]; i++) {
		//cout << dp[wlen][i] << " ";
		if (dp[wlen][i] == false) {
			//cout << endl << endl;
			return  false;
		}
	}
	//cout << endl << endl;
	return true;
}
void dfs(int ban, int dep=1, int useCnt = 0) {
	if (DP(ban) ) {
		ans = useCnt;
	}
	if (ans <= useCnt || dep > v[ban]) {
		return;
	}
	
	dfs(ban, dep+1, useCnt);
	if (used[dep] == false) {
		w[ban][wlen] = dep;
		wlen++;
		dfs(ban, dep+1, useCnt+1);
		wlen--;
	}
}

void prepare(int ban) {
	for(int i = 0; i <= v[ban]; i++)
		used[i] = false;
	for(int i = 0; i < d[ban]; i++)
		used[w[ban][i]] = true;
	ans = 5;
	wlen = d[ban];
}
int solve(int ban) {
	prepare(ban);
	dfs(ban);
	return ans;
}


signed main() {
	int i, j;
	Read("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1c\\in-c-small.txt");
	
	for( i = 0; i < t; i++ ) {
		if ( v[i] > 30 ) continue;
		int ans = solve(i);
		cout << i << " : OK" << endl;
		sprintf(out[i], "Case #%lld: %lld\n", i+1, ans);
	}
	
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1c\\out-c-small.txt", t);
	return 0;
}

//�t�@�C������(�R���\�[�����炾�Ɖ����Ȃ��ɂȂ�(Alarge�ł̋��P) )
void Read(char *name ) {
	FILE *fp = fopen(name, "r");
	if ( fp == NULL ) {
		printf("%s is not found.\n", name);
		return;
	}
	
	fscanf(fp, "%lld", &t);
	for(int i = 0; i < t; i++ ) {
		fscanf(fp, "%lld%lld%lld", c+i, d+i, v+i);
		for(int j = 0; j < d[i]; j++)
			fscanf(fp, "%lld", w[i]+j);
	}
	fclose(fp);
}

//out�̒����o��(���s�Ȃ�)
void Write(char *name, int T) {
	FILE *fp = fopen(name, "w");
	for(int i = 0; i < T; i++ ) {
		fprintf(fp, "%s", out[i]);
		fprintf(stdout, "%s", out[i]);
	}
	fclose(fp);
}