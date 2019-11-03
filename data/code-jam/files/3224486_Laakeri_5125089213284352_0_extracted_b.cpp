#include <bits/stdc++.h>
#define F first
#define S second
#define X real()
#define Y imag()
using namespace std;
typedef long long ll;
typedef long double ld;

std::mt19937 rng(time(0));
int getrand(int a, int b){
	return a+rng()%(b-a+1);
}

int pool[200];
string ss[10];
ld ct[10];

vector<int> ini;
vector<int> g[111];

string cs;

int m;
int n;

ld no = 0;

void test(){
	int pp=0;
	for (int a:ini){
		pool[pp++]=a;
	}
	string ts;
	vector<int> sq;
	ld w = 1;
	while (pp>0){
		w *= (ld)pp;
		int x=getrand(0, pp-1);
		int t=pool[x];
		swap(pool[x], pool[pp-1]);
		pp--;
		ts+=cs[t-1];
		sq.push_back(t);
		for (int nx:g[t]){
			pool[pp++]=nx;
		}
	}
	no+=w;
	for (int i=0;i<m;i++){
		if (ts.find(ss[i])!=string::npos){
			ct[i]+=w;
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
		ini.clear();
		for (int i=0;i<=105;i++){
			g[i].clear();
		}
		cin>>n;
		for (int i=1;i<=n;i++){
			int pq;
			cin>>pq;
			if (pq==0) ini.push_back(i);
			else g[pq].push_back(i);
		}
		cin>>cs;
		cin>>m;
		for (int i=0;i<m;i++){
			cin>>ss[i];
			ct[i]=0;
		}
		int iter=300000;
		no=0;
		for (int i=0;i<iter;i++){
			test();
		}
		cout<<setprecision(5)<<fixed;
		for (int i=0;i<m;i++){
			cout<<ct[i]/no;
			if (i<m-1) cout<<" ";
		}
		cout<<endl;
	}
}