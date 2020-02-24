#include <iostream>
using namespace std;
int main(){
	int Ti,Tn;
	cin>>Tn;
	for(Ti=1;Ti<=Tn;Ti++){
		int x,y,k;
		cin>>x>>y>>k;
		int nv=0,m=k,n;
		for(int i=1;i<x-1;i++){
			n=i<<1;
			nv=n;
			if(n>m)break;
			for(int j=1;j<y-1;j++){
				n+=2;
				nv+=2+i;
				if(n>m)continue;
				for(int v=0;v<4;v++){
					if(nv+v==k){
						if(n+v>m)break;
						//cout<<m<<' '<<n<<' '<<i<<' '<<j<<' '<<v<<'\n';
						m=n+v;
						if(v==0)goto next;
						else goto chkv;
					}
				}
chkv:				continue;
			}
next:			continue;
		}
		if(k<x&&k<m)m=k;
		if(k<y&&k<m)m=k;
		cout<<"Case #"<<Ti<<": "<<m<<"\n";
	}
	return 0;
}
