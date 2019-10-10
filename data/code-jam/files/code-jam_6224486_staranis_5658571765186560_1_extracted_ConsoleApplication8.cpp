// ConsoleApplication8.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	int T, X, R, C, result;
	cin >> T;
	for (int i = 1; i <= T; ++i){
		cin >> X >> R >> C;
		result = X >= 7 || R*C%X != 0;
		result |= X == 3 && (R == 1 || C == 1);
		result |= X == 4 && (R < 3 || C < 3);
		result |= X == 5 && (R < 3 || C < 3 || R*C < 20);
		result |= X == 6 && (R < 4 || C < 4);
		cout << "Case #" << i << ": " << (result ? "RICHARD" : "GABRIEL") << endl;
	}
	return 0;
}

