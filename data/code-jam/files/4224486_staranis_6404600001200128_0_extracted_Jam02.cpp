#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	int T;
	cin >> T;
	for (int i = 0; i < T; i++){
		int N, eatrate = 0, count1 = 0, count2 = 0;
		cin >> N;
		int* list = new int[N];
		for (int j = 0; j < N; j++){
			cin >> list[j];
		}
		for (int j = 1; j < N; j++){
			int diff = list[j - 1] - list[j];
			if (diff > 0){
				count1 += diff;
				if (eatrate < diff){
					eatrate = diff;
				}
			}
		}
		for (int j = 1; j < N; j++){
			if (list[j - 1] < eatrate){
				count2 += list[j - 1];
			}
			else{
				count2 += eatrate;
			}

		}
		cout << "Case #" << (i + 1) << ": " << count1 << " " << count2 << endl;
		delete[] list;
	}
	return 0;
}

