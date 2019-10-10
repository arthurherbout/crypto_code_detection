#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime> 
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define ll long long 
#define sz(a) int((a).size())
#define pb push_back
#define SORT(x) sort((x).begin(),(x).end()); 
#define VI vector<int>
#define VII vector < vector <int> >
#define MP make_pair
#define SET(x,byte) memset(x,byte,sizeof(x));
#define I(x) ((x) - 'A')
#define REP(i,mx) for(int i=0;(i)<(mx);i++)
#define RFOR(i,mx,mn) for(int i=(mx);(i)>=(mn);i--)
#define FORD(i,mn,mx,k) for(int i=(mn);(i)<=(mx);i+=(k))
#define FOR(i,mn,mx) for(int i=mn;(i)<=(mx);i++)
#define U(x) (1<<(x))
#define INF 1000000000
#define DEBUG 0

using namespace std;
int uz;
char message[] = "welcome to code jam";
int dp[501][20];
char str[501];

int yap3(int y){
	
	REP(i,uz){
		int top=0;
		if(str[i] == message[y]){
			FOR(j,i+1,uz-1) top += dp[j][y+1];
		}
		dp[i][y] = top%10000;
		/*
		if(dp[i][y] != 0){
			cout<<i<<" "<<y<<" "<<dp[i][y]<<endl; 
		}
		*/
	}
}
int yap2(){
	int mes = 17;
	//cout<<message[18]<<"KKK"<<endl;
	SET(dp,0);
	REP(i,uz) if(message[18] == str[i]) dp[i][18] = 1;
	while(mes>=0){
		yap3(mes);
		mes--;
	}
	int top=0;
	REP(i,uz) top+=dp[i][0];
	return top%10000;
}

void read(){
	uz=0;
	char ch;
	while(1){
		scanf("%c",&ch);
		str[uz++] = ch;
		if(ch=='\n') break;
	}
	uz-=1;
}

string form(int k){
	stringstream is;
	is<<k;
	string s = is.str();
	while(sz(s)<4){
		s=  "0" + s;
	}
	return s;
}

int main(){
	int t,cs=0;
	scanf("%d\n",&t);
	while(t--){
		read();
		//printf("*%s* %d",str,uz);
		int res = yap2();
		printf("Case #%d: %s\n",++cs,form(res).c_str());
	}
	return 0;
}
