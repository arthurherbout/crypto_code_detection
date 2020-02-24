#include <iostream>
#include <vector>
using namespace std;


int main()
{
    int casenum;
    cin>>casenum;

    int n;

    vector<int>v1;
    vector<int>v2;

    int cnt = 0;
    
    while(casenum--){
	cin>>n;
	
	v1 = v2 =  vector<int>(n,0);

	for(int i=0;i<n;++i){
	    cin>> v1[i];
	}

	for(int i=0;i<n;++i){
	    cin>> v2[i];
	}


	sort(v1.begin(),v1.end());
	sort(v2.begin(),v2.end());

	long long product = 0;

	for(int i=0;i<n;++i){
	    product += v1[i]*v2[n-i-1];
	}
	
	cout<<"Case #"<<++cnt<<": "<<product<<endl;
    }
    
    return 0;
}
