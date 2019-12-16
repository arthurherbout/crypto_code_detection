#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

long long int calc(int* list, int B, long long int time){
	long long int count = 0;
	for (int b = 0; b < B; b++){
		count += (time + list[b] - 1) / list[b];
	}
	return count;
}

int main()
{
	int T;
	cin >> T;
	for (int i = 0; i < T; i++){
		int B, N, result = -1;
		cin >> B >> N;
		int* Bn = new int[B];
		for (int j = 0; j < B; j++){
			cin >> Bn[j];
		}
		long long int time = 0;
		long long int inf = 0;
		long long int sup = 100000000000000;
		while (true)
		{
			time = (inf + sup) / 2;
			long long int count = calc(Bn, B, time);
			if (count > N){
				sup = time;
			}
			if (count < N){
				inf = time + 1;
			}
			if (count == N || inf == sup)
			{
				if (count != N){
					time = inf;
				}
				count = calc(Bn, B, time);
				int minr = 100000000, bcount = count - N;
				for (int b = 0; b<B; b++){
					if (Bn[b] == 1){
						minr == 1;
						break;
					}
					if (minr > time % Bn[b]){
						minr = time % Bn[b];
					}
				}
				for (int b = B - 1; b >= 0; b--)
				{
					if (Bn[b] == 1 || minr == time % Bn[b]){
						if (bcount-- == 0){
							result = b + 1;
							break;
						}
					}
				}
				break;
			}
		}
		delete[] Bn;
		cout << "Case #" << (i + 1) << ": " << result << endl;
	}
	return 0;
}

