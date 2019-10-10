#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;

int c;
int len;
long long v[50][50];
bool check(long long ans){
	if(ans%2==0 ||
		ans%3==0 ||
		ans%5==0 ||
		ans%7==0) return true;
	else return false;
}
long long cal(int row, long long sum, bool f){
	int ret=0;
	if(row==len){
		if(check(sum)) return 1;
		else return 0;
	}

	for(int i=1;i<=len-row;i++){
		ret+=cal(row+i, sum+v[row][i],false);
		if(!f) ret+=cal(row+i, sum-v[row][i],false);
	}
	return ret;
}
int main() {
	
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	cin>>c;

	for(int test=1; test<=c; test++) {
		memset(v,0,sizeof v);
		int ans=0;
		string str;
		cin>>str;
		len =str.size();
		for(int i=0;i<str.size();i++){
			for(int j=1;j<=str.size()-i;j++){
				string t=str.substr(i,j);
				sscanf(t.c_str(),"%lld",&v[i][j]);
			}
		}
		ans=cal(0,0,true);
		cout<<"Case #"<<test<<": "<<ans<<endl;
	}

	return 0;
}
