#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cstring>
using namespace std;
string v[8];
struct p{
	string a;
	int l;
	int k[8];
	int r[8][8];
	int v[8][8];
};
int r[8][8];
int main(){
	int Ti,Tn,M,N;
	cin>>Tn;
	for(Ti=1;Ti<=Tn;Ti++){
		cin>>N>>M;
		memset(r,0,sizeof(int)*8*8);
		for(int i=0;i<N;i++)cin>>v[i];
		for(int i=0;i<M;i++){
			int a,b;
			cin>>a>>b;
			a--;b--;
			r[a][b]=1;
			r[b][a]=1;
		}
		stack<p> asdasd;
		p asd;
		string re="";
		for(int i=0;i<N;i++){
			memcpy(asd.r,r,sizeof(int)*8*8);
			memset(asd.v,0,sizeof(int)*8*8);
			memset(asd.k,0,sizeof(int)*8);
			asd.a=v[i];
			asd.l=i;
			asd.k[i]=1;
			asdasd.push(asd);
			while(asdasd.size()){
				asd=asdasd.top();
				asdasd.pop();
				for(int x=0;x<N;x++){
					if(asd.k[x]!=1)goto noans;
				}
				if(re==""||re>asd.a)re=asd.a;
				continue;
noans:				if(re!=""&&re<asd.a)continue;
				int l=asd.l;
				for(int x=0;x<N;x++){
					if(asd.r[l][x]&&asd.k[x]!=1){
						p vasd=asd;
						vasd.l=x;
						vasd.v[x][l]=1;
						vasd.a+=v[x];
						vasd.k[x]=1;
						asdasd.push(vasd);
					}
					if(asd.v[l][x]){
						p vasd=asd;
						vasd.l=x;
						vasd.v[l][x]=0;
						asdasd.push(vasd);
					}
				}
			}
		}
		cout<<"Case #"<<Ti<<": "<<re<<"\n";
	}
	return 0;
}

