#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

int main(){
	int TEST; cin >> TEST;
	for(int test=1;test<=TEST;test++){
		string str; cin >> str;
		vector<int> pos;
		for(int i=0;i<str.size();i++) if(str[i]=='?') pos.push_back(i);
		string res;
		for(int i=0;i<(1<<pos.size());i++){
			string s = str;
			for(int j=0;j<pos.size();j++) s[pos[j]] = ((i>>j)&1)+'0';
			long long l=0;
			for(int j=0;j<s.size();j++) l=2*l+s[j]-'0';
			long long sqr = (long long)(sqrt(l)+1e-8);
			if(sqr*sqr==l) res = s;
		}
		printf("Case #%d: %s\n", test, res.c_str());
	}
}
