//DoArray[i] == left if �ui��ڂɁu�p���P�[�Lleft�`left+K-1���Ђ�����Ԃ��v�Ƃ����������l����B0-indexed�B
//�Ⴆ��, �T���v��1�̐����Ȃ�DoArray = {0, 5, 4}�ɂȂ�B
//���쏇�͉��ɉe�����Ȃ����Ƃ���ADoArray���\�[�g���Ă��悢�B
//���Ƃ���, �T���v��1��, DoArray = {0, 4, 5}�Ƃ����ꍇ��, 
//[---]+-++- -> ++++[-++]- -> +++++[---] -> ++++++++ �ƃp���P�[�L�����ׂčK���ɂł���B
//�����DoArray�͏���, �Ƃ��Ă悢�B
//�����DoArray = {�c, x, x, �c} (x�͔C�ӂ̐���)�݂����Ȃ͖̂��Ӗ��i�����ӏ���2��Ђ�����Ԃ��Ă����ʁj�Ȃ��Ƃ���,
//����(DoArray)����ӂɒ�܂�B�������ɂ�, �u�p���P�[�Li�����[�ɂЂ�����Ԃ����H�v���ei = 0, 1, �c�ɂ��ċ��߂�Ɗy�B
//���Ƃ�, ���ۂɂ��ׂ�+�ɂȂ������𔻒肷��΂悢�B

#include <iostream>
#include <string>
using namespace std;

int t;
string s;
int k;

int solve() {
	int i, j;
	int flipCnt = 0;
	
	for (i = 0; i < (int)s.length() - k + 1; i++) {
		if (s[i] == '-') {
			for (j = 0; j < k; j++) {
				if (s[i + j] == '-') s[i + j] = '+';
				else s[i + j] = '-';
			}
			flipCnt++;
		}
	}
	
	for (i = 0; i < s.length(); i++) {
		if (s[i] == '-') {
			return -1;
		}
	}
	return flipCnt;
}

int main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		cin >> s >> k;
		int res = solve();
		
		cout << "Case #" << id << ": ";
		if (res == -1) { cout << "IMPOSSIBLE"; }
		else { cout << res; }
		cout << endl;
	}
	return 0;
}