#include <bits/stdc++.h>
#define N 20
using namespace std;

int t;
unsigned long long act;
unsigned long long inf;
string a, b;
pair<string,string> ans;

long long parse(string a){
	long long ret = 0, k = 1;

	for(long long i = a.size()-1; i >= 0; i--){
		ret += ((a[i]-'0')*k);
		k*= (10LL);
	}
	return ret;
}

void solve(int i, string resp1, string resp2){
	if(i >= a.size()){
		//cout << resp1 << " " << resp2 << endl;
		long long x = parse(resp1);
		long long y = parse(resp2);
		long long z = parse(ans.second);
		
 		if(abs(x-y) < act){
			//cout << "ho\n";
			ans = make_pair(resp1,resp2);
			act = abs(x-y);
		}
		if(abs(x-y) == act && z > y){
			//cout << "ho\n";
			ans = make_pair(resp1,resp2);
		}
		//cout << resp1 << " " << resp2 << endl;
		
		return;
	}
	
	if(a[i] != '?' && b[i] != '?'){
		//cout << a[i] << " " << b[i] << "\n";
		solve(i+1, resp1+a[i],resp2+b[i]);
		return;
	}
	
	else if(a[i] == '?' && b[i] == '?'){
		for(int j = 0; j <= 9; j++){
			solve(i+1,resp1+char('0'+j),resp2 + '0');
		}
		for(int j = 0; j <= 9; j++){
			solve(i+1,resp1+'0',resp2 +char('0'+j));
		}
		return;
	}
	
	else if(a[i] == '?'){
		for(int j = 0; j <= 9; j++){
			solve(i+1,resp1+char('0'+j),resp2+b[i]);
		}
		return;
	}
	else if(b[i] == '?'){
		for(int j = 0; j <= 9; j++){
			solve(i+1,resp1+a[i],resp2+char('0'+j));
		}
		return;
	}
}

int main () {
	
	cin >> t;
	inf = 999999999999999999;
	act = inf;
	
	for(int caso = 1; caso <= t; caso++){
		cin >> a >> b;
		act = inf;
		ans.first = ans.second = "";
		for(int i = 0; i < a.size(); i++){
			ans.first += '9';
		}
		for(int i = 0; i < b.size(); i++){
			ans.second += '9';
		}
		cout << "Case #" << caso << ": ";
		solve(0,"","");
		cout << ans.first << " " << ans.second << endl;
	}
	
	return 0;	
}
