#include <iostream>
#include <stdint.h>
#include <cstdlib>
using namespace std;
int T,R,N,M,K;
int v[20];
int r[20];
void slv(){
	for(int i=0;i<N;i++) r[i]=2;
	bool f=0;
	for(int i=0;i<K;i++)cin>>v[i];
	int sr=1;
	for(int i=0;i<K;i++) sr*=v[i];
	srand(sr);
	for(int i=0;i<N;i++) r[i]=rand()%(M-1)+2;
end:for(int i=0;i<N;i++)cout<<r[i];
	cout<<'\n';
}
int main(){
	int n;
	cin>>n;
	for(int i=0;i<n;i++){
		cin>>R>>N>>M>>K;
		cout<<"Case #"<<i+1<<":\n";
		for(int j=0;j<R;j++)slv();
	}
	return 0;
}

