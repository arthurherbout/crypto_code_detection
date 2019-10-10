#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool is_vow(char c) {
	return (c== 'a' || c== 'e' || c== 'i' || c== 'o' || c== 'u');
}

int main(void) {
	int T;
	cin>>T;
	for (int i=0;i<T;++i){
		string s;
		cin>>s;
		int n;
		cin>>n;
		vector<int> vow;
		vector<int> cons;
		int c = 0;
		int v = 0;
		/*if (!is_vow(s[0])) {
			vow.push_back(0);
		}*/
		for (int j=0;j<s.length();++j) {
			if (is_vow(s[j])) {
				if (c!=0){
					if (c>=n) {
						cons.push_back(c);
						vow.push_back(v-c);
						v=0;
						c=0;
					}
				}
				++v;
				c=0;

			} else {
				++v;
				++c;
			}
		}
		if (c>=n) {
			cons.push_back(c);
			vow.push_back(v-c);
			v=0;
			c=0;
		} else {
			cons.push_back(0);
			vow.push_back(v);
		}
		int p=0;
		unsigned long long result = 0;
		int N=s.length();
		for (size_t j=0;j<cons.size();++j) {
			result += cons[j]==0?0:(vow[j]+1)*(N-p-n-vow[j]+1);
			for (int k=1;k<cons[j];++k) {
				if (k<=cons[j]-n) {
					result += N-p-n-vow[j]-k+1;
				} else if (j+1<cons.size() && cons[j+1]!=0) {
					result += (N-p-vow[j] - cons[j] - vow[j+1]-n+1);
				}
			}
			p+=(vow[j]+cons[j]);
		}
		cout<< "Case #"<<i+1<<": "<<result<<'\n';
	}
}