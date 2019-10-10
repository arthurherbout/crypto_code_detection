#include <iostream>
#include <utility> 
#include <vector>
#include <stdio.h>
#include <algorithm> 
#include <utility>
#include <string.h>
#include <set>
#include <math.h>

#define MAX 301

using namespace std;

char binA[1000000], binB[1000000], binC[1000000];

void deciTobin(int n, char *bin){
	
	int i = 0;
	if (!n){
		bin[0] = '0';
		bin[1] = '\0';
		return;
	}
	
	while (n != 0){
		if(n%2 != 0){
			bin[i] = '1';
		}
		else bin[i] = '0';
		n = n/2;
		i++;
	}
	
	bin[i] = '\0';
}

void binTodeci(char *bin, long &deci){
	float exp = 0;
	deci = 0;
	for (int i = 0; i < strlen(bin); i++, exp++){
		deci += (bin[i]-'0') * pow(2,exp);
	}
}

int main(){

	long a, b, t, k, deci, pairs;
	
	bool ok;

	cin >> t;
	
	for (int ii = 0; ii < t; ii++){
		cin >> a >> b >> k;
		pairs = 0;
		ok = true;
		for (int i = 0; i < a and ok; i++){
			for (int j = 0; j < b and ok; j++){
				deciTobin(i, binA);
				deciTobin(j, binB);
				if(strlen(binA) > strlen(binB)){
					for (int kk = strlen(binB); kk < strlen(binA);){
						binB[kk] = '0';
						kk++;
						binB[kk] = '\0';
					}
				}
				else{
					for (int kk = strlen(binA); kk < strlen(binB);){
						binA[kk] = '0';
						kk++;
						binA[kk] = '\0';
					}
				}
				for (int kk = 0; kk < strlen(binB); ){
					if((binA[kk]-'0') and (binB[kk]-'0'))binC[kk] = '1';
					else binC[kk] = '0';
					kk++;
					binC[kk] = '\0';
				}
				binTodeci(binC, deci);
				if(deci < k){
					pairs++;
				}
			}
		}
		printf("Case #%ld: %ld\n", ii+1, pairs);
	}
	return 0;
}