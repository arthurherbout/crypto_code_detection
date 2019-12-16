#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <queue>

//c lib
#include <cmath>
#include <cstring>
#include <cstdlib>

//alias
#define ll long long

//user defined lib
#include <primes.h>

using namespace std;

char str[50001];
char tar[50001];

int main(void){
	int ncas,ti;
	cin >> ncas;
	for(ti=1;ti<=ncas;ti++){
		/*Write Code Here*/
		int permu[16];
		int k,mini=INT_MAX;
		string s;
		cin >> k >> s;
		strcpy(str,s.c_str());
		for(int i=0;i<k;i++) permu[i] = i;
		tar[strlen(str)] = 0;

		do{
			for(int i=0;i<strlen(str)/k;i++){
				int base = i*k;
				for(int j=0;j<k;j++){
					tar[base+j] = str[base+permu[j]];
				}
			}
			char prev = '\0';
			int temp = 0;
			for(int i=0;i<strlen(str);i++){
				if(tar[i] != prev || i==0){
					temp++;
					prev = tar[i];
				}
			}
			if(temp < mini) mini = temp;
		}while(next_permutation(permu,permu+k));

		printf("Case #%d: %d\n",ti,mini);
	}
	return 0;
}
