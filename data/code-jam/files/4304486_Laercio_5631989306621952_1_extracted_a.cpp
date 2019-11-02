#include <bits/stdc++.h>
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define INF 0x3f3f3f3f
#define endl '\n'

using namespace std;

typedef long long int ll;
typedef unsigned long long int llu;

int t;
string str;
string ans = "";
		
int main (){

	cin >> t;
	int caso = 1;
	while(t-- && cin >> str){
		ans = "";
		for(int i = 0; i < str.size(); i++){
			string aux = ans;
			reverse(aux.begin(),aux.end());
			
			if(str[i]+ans < ans+str[i]){
				ans += str[i];
			}
			else{
				string x = "";
				x+=str[i];
				for(int j = 0; j < ans.size(); j++)
					x+=ans[j];
				ans = x;
			}
		}
		cout << "Case #" << caso++ << ": "; 
		cout << ans << endl;
	}


	return 0;
}