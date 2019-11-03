#include <iostream>

using namespace std;

int main(){

   int T,i,j,k;

// get test cases;
   cin >> T;
   int K[T],C[T],S[T];

// get inputs
   	for(i=0;i<T;i++){
	cin >> K[i] >> C[i] >> S[i];
	}

// output
	for(i=0;i<T;i++){
	cout << "Case #" << i+1 << ":";
	
	// check c =1
	if(C[i]==1){
		// check K > S
		if(K[i]>S[i]){
		cout <<" IMPOSSIBLE"<< '\n';
		}
		else{
		// print all
			for(j=0;j<K[i];j++){
			cout << " " << j+1;			
			}
		cout << '\n';
		}
	}
	else{
		if(K[i] ==1){
		cout << " "<< "1" << '\n';
		}
		// if K is even
		else if(K[i] % 2== 0 ){
			if(S[i]<(K[i]/2)){
			cout <<" IMPOSSIBLE"<< '\n';
			}
			else{
			k=K[i]/2;
			//int X[k];
				for(j=0;j<k;j++){
				//X[j]=((2*(j+1))-1)*(2*(j+1));
				cout << " " <<	(((2*(j))*K[i])+2*(j+1));			
				}
			cout << '\n';
			}
		}
		// else K is odd
		else{
			if(S[i]<(K[i]+1/2)){
			cout <<" IMPOSSIBLE"<< '\n';
			}
			else{
			k=(K[i]+1)/2;
			//int X[k];
				for(j=0;j<k-1;j++){
				//X[j]=((2*(j+1))-1)*(2*(j+1));
				cout << " " <<	(((2*(j))*K[i])+2*(j+1));			
				}
			cout << " " <<(((2*(j))*K[i])) << '\n';
			}
		}
	}	
	
	}
	
}


