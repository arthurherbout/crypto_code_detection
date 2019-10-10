#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

int result[5000*5];
string s_;

int f(int p, const vector<vector<string>> & d, int m ) {//m: index of the first letter I can change
	int n=1000;
	if (result[m*5000+p] != 1000)
		return result[m*5000+p];
	if (p==s_.length()) {
		result[m*5000+p] = 0;
		return 0;
	}
	string s = s_.substr(p,s_.length());
	for (int i=1;i<=min((size_t)10,s.length());++i){
		string s1 = s.substr(0,i);
		string s2 = s.substr(i,s.length());
		//try changing 0 letters
		if (find(d[i].begin(),d[i].end(),s1) != d[i].end()) {
			n = min(n,f(p+i,d,max(0,m-i)));
			continue;
		}
		//try changing one letter
		for (int j=m;j<i;++j) {
			for (auto it = d[i].begin();it!=d[i].end();++it) {
				bool found = true;
				for (int pos=0;pos<i && found;++pos) {
					if (pos==j)
						continue;
					if ((*it)[pos]!=s1[pos]) {
						found = false;
						continue;
					}
				}
				if (found) {
					n = min(n,1+f(p+i,d,max(0,4-(i-j-1))));
					break;
				}
			}
			for (int k=j+5;k<i;++k) {		
				for (auto it=d[i].begin();it!=d[i].end();++it){
					bool found = true;
					for (int pos=0;pos<i && found;++pos) {
						if (pos==j || pos == k)
							continue;
						if ((*it)[pos]!=s1[pos]) {
							found = false;
							continue;
						}
					}
					if (found) {
						n = min(n,2+f(p+i,d,max(0,4-(i-k-1))));
						break;
					}
				}
			}
		}
	}
	result[m*5000+p] = n;
	return n;
}

int main(void) {
	vector<vector<string>> d(11);
	ifstream df("garbled_email_dictionary.txt");
	size_t max_size = 0;
	while(!df.eof()) {
		string s;
		df>>s;
		d[s.length()].push_back(s);
		max_size = max(max_size,s.length());
	}
	//cout<<"max size:"<<max_size<<'\n';
	int T;
	cin>>T;
	for (int t=0;t<T;++t)  {
		for (int i=0;i<5000*5;++i)
			result[i]=1000;
		//string s;
		cin>>s_;
		//cout<< s<<'\n';
		cout<<"Case #"<< t+1<<": "<<f(0,d,0)<<'\n';
	}
	return 0;
}

