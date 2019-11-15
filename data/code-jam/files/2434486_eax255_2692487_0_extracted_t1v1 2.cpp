#include <iostream>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;
int main(){
	int lkm;
	cin>>lkm;
	vector<int> par;
	stack<int> vli;
	int a,n;
	for(int cas=1;cas<=lkm;cas++){
		par.clear();
		while(!vli.empty()) vli.pop();
		cin>>a>>n;
		for(int i=0;i<n;i++){
			int v;
			cin>>v;
			par.push_back(v);
		}
		sort(par.begin(),par.end());
		int r=0;
		int rm=n;
		int i=0;
		vli.push(r);
		vli.push(i);
		vli.push(a);
		while(!vli.empty()){
			a=vli.top();
			vli.pop();
			i=vli.top();
			vli.pop();
			r=vli.top();
			vli.pop();
			if(r>rm)continue;
			while(i<n){
				int v=par[i];
				if(v<a){
					a+=v;
					i++;
				}
				else{
					vli.push(r+1);
					vli.push(i);
					vli.push(2*a-1);
					while(i<n){r++;i++;}
				}
			}
			if(r<rm)rm=r;
		}
		cout<<"Case #"<<cas<<": "<<rm<<"\n";
	}
	return 0;
}
