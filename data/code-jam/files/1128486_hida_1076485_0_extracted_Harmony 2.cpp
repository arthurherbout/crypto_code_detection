#include <iostream>
#include <math.h>

#define NMAX 100

using namespace std;

int main(void){
	int T,N,L,H;
	int freq[NMAX];
	int answer;
	int i,j,k;

	cin >> T;
	for(i=1;i<=T;i++){
		cin >> N;
		cin >> L;
		cin >> H;
		for(j=0;j<N;j++){
			cin >> freq[j];
		}

		answer = 0;
		for(j=L;j<H+1;j++){
			for(k=0;k<N;k++){
				if((freq[k] % j != 0) && (j % freq[k] != 0)){
					break;
				}
			}
			if(k == N){
				answer = j;
				break;
			}
		}
		cout << "Case #" << i << ": ";
		if(answer){
			cout << answer << endl;
		}else{
			cout << "NO" << endl;
		}
	}

	return 0;
}
