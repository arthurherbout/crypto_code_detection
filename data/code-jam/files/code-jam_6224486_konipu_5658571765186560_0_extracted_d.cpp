#include<iostream>
using namespace std;
int main () {
	int t;
	cin >> t;
	for(int k=1;k<=t;k++){
		int x,r,c;
		cin >> x >> r >> c;
		if((r*c)%x!=0){
			cout<<"Case #"<<k<<": "<<"RICHARD"<<endl;
			continue;
		}
		if(x==1||x==2){
			cout<<"Case #"<<k<<": "<<"GABRIEL"<<endl;
			continue;
		}
		if(x==3){
			int m;
			m=min(r,c);
			if(m==1){
				cout<<"Case #"<<k<<": "<<"RICHARD"<<endl;
				continue;
			}else{
				cout<<"Case #"<<k<<": "<<"GABRIEL"<<endl;
				continue;
			}
		}
		if(x==4){
			int m;
			m=min(r,c);
			if(m<=2){
				cout<<"Case #"<<k<<": "<<"RICHARD"<<endl;
				continue;
			}else{
				cout<<"Case #"<<k<<": "<<"GABRIEL"<<endl;
				continue;
			}
		}
	}
	return 0;
}
