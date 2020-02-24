//���ǉ��F2����00���A�l�@���ԁF5���A���1��

//�li(1 <= i <= D)��Pi�̃p���P�[�L���ŏ������Ă���B���̑������l�̐l��0�̃p���P�[�L�������Ă���B
//�e���A����2�̓��ǂ��炩1�̍s�����Ƃ邱�Ƃ��ł���B
//�E�p���P�[�L�������Ă���l�S�����A���ꂼ��̎����Ă���p���P�[�L��1���炷
//�E�p���P�[�L�������Ă���N����l(�����A�Ƃ���), �C�ӂ̈�l(�����B�Ƃ���)��I�ԁB���̎�A��B�Ƀp���P�[�L��C�ӌn���B
//�S�Ẵp���P�[�L���Ȃ��Ȃ�܂łɂ�����ŏ��̎��Ԃ́H
//1 <= D <= 1000, 1 <= Pi <= 1000
//small: 1 <= D <= 6, 1 <= Pi <= 9

//�l�@(�����lOK�ȑ���ƁA1to1��1�񐧌��ōs�����삪����̂ŁA�Ɨ��ɍl������ۂ����Ƃ͓�����ہ[��)
//�E�p���P�[�L��n���n�̂�c�p���P�[�L��H�ׂ�O�ɑS������Ă��܂��������ǂ��B

//���A�Ɨ��ɍl��������ۂ��B

//�E�ŒZ���Ԃ́c�p���P�[�L��n���n�̓z��������� + �p���P�[�L��n���n�̓z��S�čs������ɂ�����PMAX( PMAX is �ő�p���P�[�L�����L���Ă���l�̎��p���P�[�L�̌�)
//�p���P�[�L��n���񐔂����߂��Ƃ��́APMAX���ŏ���������ɂȂ����B����(������헪)���l����B

//�E�p���P�[�L���󂯎��l�FEmpty�Ȑl���ǂ��ˁB
//�E�p���P�[�L��n���l�F�×~�ɍ�PMAX�p���P�[�L�����l���ǂ������B
//�E�p���P�[�L�̕������F�s���B1�`Pi-1�ɂ��đS�Ď������B�������͌�ŁB
//�E�p���P�[�L�𕪂���ׂ����ǂ����c�p���P�[�L��n���񐔂ɒB���Ă��Ȃ���Ε�����I�����������[���ɂ��Ă���̂ŊȒP�I�I

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

//n��k�ȉ��ɂ���̂ɕK�v�ȁu������v����̉񐔂̍ŏ��l��Ԃ�
int DP(int n, int k, int dep=0) {
	//cout << n << " " << k << endl;
	
	if ( n <= k )
		return 0;
		
	int ret = 114514;
	for( int i = 1; i < n; i++ ) {
		ret = min(ret, DP(i, k, dep+1) + DP(n-i, k, dep+1) + 1 );
	}
	return ret;
}

signed main() {
	int i, j;
	cin >> t;
	for( i = 0; i < t; i++ ) {
		int d, p[1000];
		cin >> d;
		for( j = 0; j < d; j++ )
			cin >> p[j];
		
		int ans = 114514;
		for( int num = 0; num < 10; num++ ) {
			int sum = 0;
			for( j = 0; j < d; j++ )
				sum += DP(p[j], num);
			ans = min(ans, sum + num);
		}
		
		sprintf(out[i], "Case #%d: %d\n", i+1, ans);
	}
	
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\qual\\out-b-small.txt", t);
	return 0;
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