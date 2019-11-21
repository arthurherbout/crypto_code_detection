//�e���������X1�炵���i���m����orz�j�B
//��s���Ȃ��ꍇ, �e�s�Ɨ��Ɍ��߂���B���ɐL�΂��邾���L�΂��×~�Ƃ�, ������K���ɂ��ΊȒP�Ɍ��߂���B
//�r���ɋ�s������ꍇ��, �ׂ荇���s�������Ă��銴����OK�B
//�ŏ��Ɂu1�����ȏ゠��s�v�������Ă���B���������Ɍ������Č��߂�, ���Ɍ������Č��߂�B�e�s�͓K���ɂ��B
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
		if (j >= ss.length()) break;	//������ӂ̏����������Ƃ��Ȃ��悤�ɁI�I
		
		//[i, j]��ss[j]�ɏ���������
		for (int k = i; k <= j; k++) {
			ss[k] = ss[j];
		}
		
		//�J�[�\���ړ�
		i = j + 1;
	}
	
	//�Ō��???�𖄂߂�
	char c;
	for (i = (int)ss.length() - 1; i >= 0; i--) {
		if (ss[i] != '?') { c = ss[i]; break; }
	}
	for (i = 0; i < ss.length(); i++) {
		if (ss[i] == '?') ss[i] = c;
	}
	
	//�K���ɂ͂� (�l�R�s�[)
	s[row] = ss;
}

int main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		int i, j;
		
		cin >> h >> w;
		for (i = 0; i < h; i++) cin >> s[i];
		
		//����
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
			if (j == w) {	//�R�s�[
				for (j = 0; j < w; j++) s[i][j] = s[i + 1][j];
			}
			else {	//�͂�
				solve(i);
			}
		}
		for (i = startRow + 1; i < h; i++) {
			for (j = 0; j < w; j++) {
				if (s[i][j] != '?') break;
			}
			if (j == w) {	//�R�s�[
				for (j = 0; j < w; j++) s[i][j] = s[i - 1][j];
			}
			else {	//�͂�
				solve(i);
			}
		}
		
		//������
		cout << "Case #" << id << ":" << endl;
		for (i = 0; i < h; i++) cout << s[i] << endl;
	}
	
	return 0;
}