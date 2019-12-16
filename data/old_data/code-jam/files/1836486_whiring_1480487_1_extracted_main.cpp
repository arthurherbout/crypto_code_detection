#include <stdio.h>
#include <iostream>

using namespace std;

int main(){
  int loop_num,N,oldN,sum,allsum;
  int mark[200],check[200];
  double ll[200];
  
  cin >> loop_num;
  
  
  for(int i=0;i<loop_num;i++){
    sum = 0;
    cin >> N;
    oldN = N;
    for(int j=0;j<oldN;j++){
      cin >> mark[j];
      sum += mark[j];
      //      cout << mark[j];
    }
    allsum = 2*sum;
    double threshold = (double)(allsum)/(double)(oldN);
    // cout << threshold << endl;
    
    
    for(int j=0;j<oldN;j++){
      if(threshold < (double)mark[j]){
	N--;
	allsum -= mark[j];
	ll[j] = 0.0;
	check[j] = 0;
	//	cout << endl;
      }
      else{
	check[j] = 1;
	//	cout << endl;
      }
    }
    //    cout << sum << endl;
    threshold = (double)(allsum)/(double)N;
    //        cout <<"new" <<  threshold << endl;
    for(int j=0;j<oldN;j++){
      if(check[j]){
	//	cout << mark[j] << endl;
	ll[j] = 100*(threshold - mark[j])/(double)sum;
      }
      
    }
    
    cout << "Case #" << (i + 1) << ": ";
    for(int j=0;j<oldN-1;j++){
      //      cout << ll[j] <<" ";
      printf("%10f ",ll[j]);
    }
    //    cout << ll[oldN-1] << endl;
    printf("%10f\n",ll[oldN-1]);
  }
  
}
