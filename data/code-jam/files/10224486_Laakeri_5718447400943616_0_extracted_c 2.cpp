#include <bits/stdc++.h>
#define F first
#define S second
#define X real()
#define Y imag()
using namespace std;
typedef long long ll;
typedef long double ld;

int n;

int fail=0;

void go(vector<int>&can, int wf, int mf){
	int f=0;
	if (wf==0&&mf==0) return;
	if (fail) return;
	
	for (int i=0;i<n;i++){
		if (((1<<i)&wf)>0){
			for (int j=0;j<n;j++){
				if (((1<<j)&mf)>0&&((1<<j)&can[i])>0){
					f=1;
					go(can, (1<<i)^wf, (1<<j)^mf);
				}
			}
		}
	}
	if (f==0) {
		fail=1;
	}
}

int bv=1e9;

int ok(vector<int> can){
	fail=0;
	go(can, (1<<n)-1, (1<<n)-1);
	if (fail) return 0;
	return 1;
}

void go(vector<int> can, int i, int j, int a){
	if (a>=bv) return;
	if (ok(can)){
		bv=a;
		return;
	}
	if (j<n&&i<n) {
		go(can, i, j+1, a);
		if ((can[i]&(1<<j))==0){
			vector<int> nc=can;
			nc[i]^=(1<<j);
			go(nc, i, j+1, a+1);
		}
	}
	else{
		if (i<n){
			go(can, i+1, 0, a);
		}
	}
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	for (int tc=1;tc<=tcs;tc++){
		cout<<"Case #"<<tc<<": ";
		cin>>n;
		vector<int> can(n);
		for (int i=0;i<n;i++){
			string s;
			cin>>s;
			for (int j=0;j<n;j++){
				if (s[j]=='1'){
					can[i]^=(1<<j);
				}
			}
		}
		bv=n*n;
		go(can, 0, 0, 0);
		cout<<bv<<endl;
	}
}