#include <iostream>
#include <string>
using namespace std;
int tl[1<<22];
int tb[1<<22];
int main(){
	int T;
	cin>>T;
	for(int Ti=1;Ti<=T;Ti++){
		cout<<"Case #"<<Ti<<": ";
		string v;
		int n;
		cin>>v>>n;
		int nv=v.size();
		for(int i=0;i<v.size();i++){
			if(v[i]=='a'||v[i]=='e'||v[i]=='i'||v[i]=='o'||v[i]=='u') v[i]=0;
			else v[i]=1;
			tl[i]=v[i];
			tb[i]=!tl[i];
		}
		for(int i=1;i<nv;i++){
			if(tl[i])tl[i]+=tl[i-1];
			if(tb[i])tb[i]+=tb[i-1];
		}
		int r=0;
		int k=1;
		tb[nv]=0;
		for(int i=nv-1;i>=0;){
			if(tl[i]>=n){r+=(i-n+2)*k;i--;k=1;}
			else if(tl[i]){k+=tl[i];i-=tl[i];}
			else if(tb[i]){k+=tb[i];i-=tb[i];}
			else break;//error
		}
		cout<<r<<'\n';
	}
	return 0;
}
