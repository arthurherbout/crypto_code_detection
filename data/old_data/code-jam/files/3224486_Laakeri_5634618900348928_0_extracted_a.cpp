#include <bits/stdc++.h>
#define F first
#define S second
#define X real()
#define Y imag()
using namespace std;
typedef long long ll;
typedef long double ld;

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	for (int tc=1;tc<=tcs;tc++){
		cout<<"Case #"<<tc<<": ";
		string s;
		cin>>s;
		int v=0;
		vector<char> lol;
		for (int i=0;i<(int)s.size();i++){
			if ((int)lol.size()>0&&lol.back()==s[i]){
				v++;
				lol.pop_back();
			}
			else {
				lol.push_back(s[i]);
			}
		}
		cout<<((int)s.size()/2)*5+v*5<<endl;
	}
}