#include <iostream>
#include <cmath>
using namespace std;
int v[1000];
int main(){
	int T,N;
	double e_sorted=0;
	cin>>T;
	for(int Ti=1;Ti<=T;Ti++){
		cin>>N;
		for(int i=0;i<N;i++)cin>>v[i];
		double e0=0,e1=0;
		double Nd=N;
		e_sorted=-log(Nd)/N*(N-1);
		for(int i=0;i<N;i++)e0+=abs(i-v[i])/Nd*log(abs(i-v[i]+.0000001)/Nd);
		for(int i=0;i<N-1;i++)e1+=abs(v[i]-v[i+1])/Nd*log(abs(v[i]-v[i+1])/Nd);
		//cout<<e0<<' '<<e1<<' '<<e_sorted<<'\n';
		if(isnan(e0)) goto bad;
		if(floor(e1*1000000)==floor(e_sorted*1000000))goto bad;
		//cout<<e0/e1<<'\n';
		if(e0/e1<1)goto bad;
good:		cout<<"Case #"<<Ti<<": GOOD\n";
		continue;
bad:		cout<<"Case #"<<Ti<<": BAD\n";
	}
	return 0;
}

