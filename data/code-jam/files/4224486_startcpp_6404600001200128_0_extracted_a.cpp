//�@�uAi���C�ӌH�ׂ遨�ҋ@��Ai+1�ɂȂ�悤�ɐ���t���v�Ƃ����Ƃ��ɁA�H�ׂ����͍ŏ��ł�����H
//�A�uAi��min(Ai, v * 10)�H�ׂ遨�ҋ@��Ai+1�ɂȂ�悤�ɐ���t���v�Ƃ����Ƃ��ɁA�H�ׂ����͍ŏ��ł�����H
//���𗝉�����܂łɂ����������ԁF22��
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

signed main() {
	int i, j;
	cin >> t;
	
	int n, m[1000];
	for( i = 0; i < t; i++ ) {
		cin >> n;
		for( j = 0; j < n; j++ )
			cin >> m[j];
		
		//�@
		int y = 0;
		for( j = 0; j < n-1; j++ ) {
			if ( m[j] > m[j+1] )
				y += m[j] - m[j+1];
		}
		
		//�A
		int z;
		for(int v = 0; ; v++ ) {
			int plate = m[0], ate = 0;
			for( j = 1; j < n; j++ ) {
				int tmp = min(plate, v);
				plate -= tmp;
				ate += tmp;
				if ( plate > m[j] )
					break;
				plate = m[j];
			}
			if ( j == n ) {
				z = ate;
				break;
			}
		}
		
		sprintf(out[i], "Case #%d: %d %d\n", i+1, y, z);
	}
	
	Write("C:\\Users\\Kawakami\\kyopro\\GCJ\\2015\\r1\\out-a-small.txt", t);
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