// number.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <iostream>
using namespace std;
char* num[31]={
	"","","027","143","751","935","607","903","991","335","047","943","471","055","447","463","991","095","607","263","151","855","527","743","351","135","407","903","791","135","647"};
int main(int argc, char* argv[])
{
	int i,j,testCases;
	cin >> testCases;
	for (i = 1; i <= testCases;i ++)
	{
		cin >> j;
		cout << "Case #" << i << ": " << num[j] << "\n";
	}
	return 0;
}

