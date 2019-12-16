#include <iostream>
#include <string>

using namespace std;

int main(){
  // get inputs
    int T,i,j,a,L;
    cin >> T;
    int K[T];
    const char *x;
    string w;
    string S[T];
    for(i=0;i<T;i++){
        cin >> w;
	S[i]=w;
    }
  
  // convert inputs to char array
    for(i=0;i<T;i++){
	w=S[i];
 	a=0;
	L=w.length();
	for(int j=0;j < (L-1);j++){
	   	if(w[j]==w[j+1]){
			
	   	}
		else{
		a++;
		}
	}
	if(w[L-1]=='+'){
	cout << "Case #" << i+1 << ": " << a << endl;	
	}
	else{
	cout << "Case #" << i+1 << ": " << a+1 << endl;	
	}
    }	

return 0;
}
