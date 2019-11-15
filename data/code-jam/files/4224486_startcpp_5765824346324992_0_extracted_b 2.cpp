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
	//��O(n[tnum]logb[tnum])
	//�D��x(���ɐ؂鎞�ԁ��ԍ�)
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

//X�̒����́AO(M(MIN)/B * N)���炢�K�v�ɂȂ�̂ŁAsmall�������Ȃ��B
int solve2(int tnum) {
	static int X[1000001];	//i���ڊJ�n���ɃJ�b�g���I������q�̐���X[i]�Ƃ���B
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
	//time���ڂɃJ�b�g���n�߂���l�̓��Anum�Ԗڂɔԍ��̏������l��T��(������1�Ԗڂɏ����� is �ŏ��Ƃ���)
	for( i = 0; i < B; i++ ) {
		if ( time % m[tnum][i] == 0 )
			num--;
		if ( num == 0 )
			break;
	}
	return i+1;
}

//x���ڏI�����ɉ��l�̐l�̎����؂�ꂽ����Ԃ�
int cuted(int tnum, int x) {
	int i;
	int ret = 0;
	if ( x < 0 )
		return 0;
	for( i = 0; i < b[tnum]; i++ )
		ret += x/m[tnum][i] + 1;
	return ret;
}

//time�����܂�Ίy�����ƕ��������B
//�e�l�ɒ��ڂ���ƁA�li��time���܂ł�time/m[i]�l���U�����Ă���B
int solve3(int tnum) {
	int st = 0, ed = 10000000000010, medi;
	int res;
	
	//for(int i = 0; i < 30; i++ )
	//	cout << cuted(tnum, i) << endl;
	
	//cuted(tnum, x) >= n[tnum]�ƂȂ�ŏ���x��T��
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
	//time���ڂɃJ�b�g���n�߂���l�̓��Anum�Ԗڂɔԍ��̏������l��T��(������1�Ԗڂɏ����� is �ŏ��Ƃ���)
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
	Read("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1\\in-b-small.txt");
	
	for( i = 0; i < t; i++ ) {
		int ans = solve3(i);
		cout << i << " : OK" << endl;
		sprintf(out[i], "Case #%lld: %lld\n", i+1, ans);
	}
	
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1\\out-b-small.txt", t);
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
		fscanf(fp, "%lld%lld", &b[i], &n[i]);
		for(int j = 0; j < b[i]; j++ )
			fscanf(fp, "%lld", &m[i][j]);
	}
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