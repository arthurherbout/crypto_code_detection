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
int v[100];
int cnt[10][100];

string nm[10];

int main (){
	
	cin >> t;
	
	nm[0] = "ZERO";
	nm[1] = "ONE";
	nm[2] = "TWO";
	nm[3] = "THREE";
	nm[4] = "FOUR";
	nm[5] = "FIVE";
	nm[6] = "SIX";
	nm[7] = "SEVEN";
	nm[8] = "EIGHT";
	nm[9] = "NINE";
	int caso = 1;
	for(int i = 0; i <= 9; i++){
		for(int j = 0; j < nm[i].size(); j++){
			cnt[i][nm[i][j]-'A']++;
		}
	}
	
	while(t--){
		cin >> str;
		memset(v,0,sizeof v);
		vector<int> ans;
		
		for(int i = 0; i < str.size(); i++){
			v[str[i]-'A']++;
		}
		for(int i = 0; i <= 9; i++){
			int aux = 10000;
			for(int j = 0; j < nm[i].size(); j++){
				aux = min(aux,v[nm[i][j]-'A']/cnt[i][nm[i][j]-'A']);
			}
			
			for(int j = 0; j < aux; j++)
				ans.pb(i);
		}
		cout << "Case #" << caso++ << ": ";
		for(int i = 0; i < ans.size(); i++){
			cout <<ans[i];
		}
		cout << endl;
	}
	
	
	return 0;
}
