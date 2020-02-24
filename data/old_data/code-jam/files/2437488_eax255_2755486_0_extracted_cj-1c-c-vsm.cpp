#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;
int fd[1<<21];
int fd2[1<<21];
int nd[11];
int nn[11];
int nw[11];
int ne[11];
int ns[11];
int ndd[11];
int ndp[11];
int nds[11];
int att[676062*100];
int atti[676062];
int main(){
	int T;
	cin>>T;
	for(int Ti=1;Ti<=T;Ti++){cerr<<Ti<<'\n';
		cout<<"Case #"<<Ti<<": ";
		memset(fd,0,(1<<21)*4);
		int n;
		cin>>n;
		for(int i=0;i<n;i++)cin>>nd[i]>>nn[i]>>nw[i]>>ne[i]>>ns[i]>>ndd[i]>>ndp[i]>>nds[i];
		memset(att,0,6760610*4);
		memset(atti,0,676061*4);
		for(int i=0;i<n;i++){
			att[nd[i]*10+atti[nd[i]]]=i;
			atti[nd[i]]++;
		}
		int r=0;
		for(int i=0;i<676061;i++){
			if(atti[i]==0) continue;
			memcpy(fd2,fd,(1<<21)*4);
			for(int j=0;j<atti[i];j++){
				int v=att[i*10+j];
				bool s=0;
				for(int k=nw[v];k<=ne[v];k++){
					if(fd[k+2000]<ns[v])s=1;
					if(fd2[k+2000]<ns[v]){fd2[k+2000]=ns[v];}
				}
				if(s) r++;
				nn[v]--;
				if(nn[v]==0) continue;
				nd[v]+=ndd[v];
				nw[v]+=ndp[v];
				ne[v]+=ndp[v];
				ns[v]+=nds[v];
				att[nd[v]*10+atti[nd[v]]]=v;
				atti[nd[v]]++;
			}
			memcpy(fd,fd2,(1<<21)*4);
		}
		cout<<r<<'\n';
	}
	return 0;
}
