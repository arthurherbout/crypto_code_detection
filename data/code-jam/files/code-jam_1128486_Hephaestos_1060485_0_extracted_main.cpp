#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main(void) {
	int T;
	cin>>T;
	for (int i=0;i<T;++i){
		int R,C;
		cin>>R>>C;
		char c;
		vector<string> ans;
		for (int j=0;j<R;++j) {
			string s;
			cin>>s;
			ans.push_back(s);
		}
		bool impossible =  false;
		for (int j=0;j<R;++j) {
			if (impossible)
				break;
			for (int k=0;k<C;++k) {
				if (ans[j][k]=='#') {
					if (j==(ans.size()-1) || (k==ans[j].length()-1) || ans[j+1][k]!='#' || ans[j][k+1]!='#' || ans[j+1][k+1] !='#') {
						impossible = true;
						break;
					}
					ans[j][k] = '/';
					ans[j+1][k]= '\\';
					ans[j][k+1] = '\\';
					ans[j+1][k+1] = '/';
				}
			}
		}
		cout<<"Case #"<<i+1<<":\n";
		if (impossible)
			cout<<"Impossible\n";
		else {
			for(int j=0;j<R;++j)
				cout<<ans[j]<<'\n';
		}
	}
}