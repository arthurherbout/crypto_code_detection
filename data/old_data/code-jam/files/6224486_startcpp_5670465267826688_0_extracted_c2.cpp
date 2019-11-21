//����L�̕����񂪗^������̂ŁA�����X��J��Ԃ���������ɂ����āAijk�Ƃ������т���邱�Ƃ��ł��邩�𔻒肹��B
//�Ȃ��A�ϊ��\(2��1��)�͗^���炦��(�����4�����Ɋ�Â�)
//�𒼉�(���������A������v�Z���Ă��ǂ����A�ǂ�����v�Z���Ă��ǂ�), ��������؂�ꏊ��ς�����Ⴄ3�������o�Ă��邱�Ƃ����邵�A
//ijk = 1����kji = 1�����藧�̂Łu1�v�ɂ܂Ƃ߂�̂�NG�Bijk��=�Ō��΂����̂ł��ŏI�I��ijk�ɂ͂ł��Ȃ��ꍇ������̂Œ��ӁB
//�������A(-i)(-j)k = ijk�Ƃ͂ł���̂ŁA(-i)(-j)k��OK�ł���B�v����ɁA1������2�����̕ϊ����֎~���������Łuijk�v���o�������銴���̂���B
//���Ȃ킿�ujkkik�v�́u(-i)(-j)k�v���uijk�v�̂悤�ɕό`�ł���̂�YES.
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

//�ϊ��\(�����Ȃ�. 1�͊ȒP�̂���h�Ƃ��Ă���)
char *conv[4] = {"hijk", "ihkj", "jkhi", "kjih"};
//�����X���b�v�̗L��
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
			
			//(1, i], (i+2, j], (j+2, S.length() - 1]�̂��ꂼ�ꂪi, j, k�ƂȂ邩�𒲍�
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

//out�̒����o��(���s�Ȃ�)
void Write(char *name, int T) {
	FILE *fp = fopen(name, "w");
	for(int i = 0; i < T; i++ ) {
		fprintf(fp, "%s", out[i]);
		fprintf(stdout, "%s", out[i]);
	}
	fclose(fp);
}