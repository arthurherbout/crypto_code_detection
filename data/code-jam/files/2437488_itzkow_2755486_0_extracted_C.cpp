#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;
typedef long long LL;


ifstream fin ("C:\\CodeJam\\Round1C\\C\\C-small-attempt1.in");
ofstream fout ("C:\\CodeJam\\Round1C\\C\\C-small-attempt1.out");

int testCase = 0;
//di, ni, wi, ei, si, delta_di, delta_pi and delta_si

int DI[1000];
int NI[1000];
int WI[1000];
int EI[1000];
int SI[1000];
int DELTA_DI[1000];
int DELTA_PI[1000];
int DELTA_SI[1000];

int ATTACK_D[1000000];
int ATTACK_W[1000000];
int ATTACK_E[1000000];
int ATTACK_S[1000000];
bool ATTACK_WAS[1000000];

int WALL[10000];
int HALF = 5000;



void Solve()
{
	
	int N,i,j,k,attacks,w,e;

	fin>>N;

	for (i = 0;i < N; i++) {
		fin>>DI[i]>>NI[i]>>WI[i]>>EI[i]>>SI[i]>>DELTA_DI[i]>>DELTA_PI[i]>>DELTA_SI[i];
	}

	j = 0;
	for (i = 0;i < N; i++) {

		for (k = 0; k < NI[i]; k++) {
			ATTACK_D[j] = DI[i]+k*DELTA_DI[i];
			ATTACK_W[j] = WI[i]+k*DELTA_PI[i];
			ATTACK_E[j] = EI[i]+k*DELTA_PI[i];
			ATTACK_S[j] = SI[i]+k*DELTA_SI[i];
			ATTACK_WAS[j] = false;

			j++;
		}
	}
	attacks = j;




	int res = 0;
	memset(WALL,0,sizeof(WALL));
	


	while (true)
	{


		int next_attack = -1;
		int next_day = -1;
		for (i = 0;i <attacks;i++)	{

			if (ATTACK_WAS[i])
				continue;

			if (next_attack == -1) {
				next_attack = i;
				next_day = ATTACK_D[i];
				continue;
			}
			if(ATTACK_D[i] < next_day) {
				next_attack = i;
				next_day = ATTACK_D[i];
				continue;
			}

		}
		if (next_day == -1)
			break;


		int new_height = 0;

		for (i = 0; i <attacks; i++) {

			if(ATTACK_WAS[i])
				continue;
			if (ATTACK_D[i] != next_day)
				continue;

			bool success = false;
			for (w = ATTACK_W[i]; w < ATTACK_E[i]; w++) {
				if (WALL[w+HALF] < ATTACK_S[i]) {
					success = true;

					break;
				}
			}
		
			ATTACK_WAS[i] = true;
			if (!success)
				continue;

			res++;
			new_height = max(new_height, ATTACK_S[i]);

		}
		for (i = 0; i <attacks; i++) {
			if (ATTACK_D[i] != next_day)
				continue;
			for (w = ATTACK_W[i]; w < ATTACK_E[i]; w++) {
				if (WALL[w+HALF] < ATTACK_S[i]) {
					WALL[w+HALF] = ATTACK_S[i];
				}
			}

		}


	}



	fout << "Case #" << testCase << ": "<<res<<endl;

}




void main()
{

	fout.precision(15);
	cout.precision(15);
	

	int TestCases = 0;
	fin>>TestCases;

	for (testCase = 1; testCase <= TestCases; testCase++) {
		
		Solve();

	}

	cout<<"finished"<<endl;
	getchar();

}



