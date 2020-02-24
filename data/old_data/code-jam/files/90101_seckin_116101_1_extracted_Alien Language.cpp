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
char ch;
int tokens[16];
vector<string> words;
char str[100];
int l,d,n;

inline string readtoken(){
	string s="";
	int say=0;
	while(1){
		scanf(" %c",&ch);
		//cout<<"suan"<<s<<" "<<ch<<endl;
		if(say == 0 && ch!='(') {s = ch; return s;}
		if (ch ==')') return s;
		if(ch!='(') s+=ch;
		say++;
	}
}
int main(){
	int cs=0;
	scanf("%d%d%d",&l,&d,&n);
	REP(i,d){
		scanf("%s",str);
		string s(str);
		words.pb(s);
	}
	REP(j,n){
		REP(i,l){
			string s = readtoken();
			//cout<<"token: "<<s<<endl;
			int mask = 0;
			REP(k,sz(s)){
				mask |= (1<< (s[k] - 'a'));
			}
			tokens[i] = mask;
		}
		int top=0;
		REP(i,d){
			int done=1;
			REP(k,l){
				if( (tokens[k] & (1<<(words[i][k] - 'a'))) == 0) {done=0;break;}
				//cout<<k<<" "<<tokens[k]<<" "<<words[i]<<endl;
			}
			top+=done;
		}
		cs++;
		printf("Case #%d: %d\n",cs,top);
	}
	return 0;
}
