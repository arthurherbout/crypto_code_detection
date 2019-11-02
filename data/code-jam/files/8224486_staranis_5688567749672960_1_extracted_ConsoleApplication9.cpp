#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

int count(long long int value){
	if (value < 20)
		return value;
	if (value % 10 == 0)
		return count(value - 1) + 1;
	int keta, result = 0;
	long long int tmp = value;
	string s = to_string(value);
	bool flag = true;

	for (int i = 1; i <= 14; i++){
		value /= 10;
		if (value == 0){
			keta = i;
			break;
		}
	}
	if (keta % 2 == 1){
		result = s[keta / 2] - '0';
	}
	for (int i = keta / 2 - 1; i >= 0; i--){
		result *= 10;
		result += s[i] - '0';
		result += s[keta - 1 - i] - '0';
		flag &= s[i] == (i == 0 ? '1' : '0');
	}
	long long int next = 0;
	for (int i = 0; i < keta - 1; ++i){
		next = next * 10 + 9;
	}
	return result + 1 + count(next) - (flag ? 1 : 0);
}


int _tmain(int argc, _TCHAR* argv[])
{
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i){
		long long int result = 0, num;
		cin >> num;
		cout << "Case #" << i << ": " << count(num) << endl;
	}
}

