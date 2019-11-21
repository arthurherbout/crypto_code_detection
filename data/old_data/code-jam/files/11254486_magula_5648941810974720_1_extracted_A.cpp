#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <cassert>
using namespace std;

typedef long long LL;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii > vii;
typedef queue<int> qi;
typedef set<int> si;

#define RE(i,b) for(int i=0; i<(int)(b); i++)
#define REP(i,a,b) for(int i=(a); i<(int)(b); i++)
#define BREP(i,a,b) for(int i=(a)-1; i>=(b); i--)
#define FE(i,X) for(typeof((X).begin()) i=(X).begin(); i!=(X).end(); ++i)

#define INF 1000000000
#define MP make_pair
#define FI first
#define SE second
template<class t> void setmin(t &a, t b){a=min(a,b);}

string X[10]={"TWO","SIX","SEVEN","FOUR","ZERO","ONE","NINE","THREE","FIVE","EIGHT"};
int clear[10]={2,6,7,4,0,1,9,3,5,8};
char Y[10]={'W','X','S','U','Z','O','N','R','V','H'};

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		string s; char c[10000];scanf("%s ",c);
		s=c;
		int O[1000];fill_n(O,1000,0);
		RE(i,s.length()){
			O[s[i]]++;
// 			printf("O %c\n",s[i]);
		}
		vi V;
		RE(i,10){
			while(O[Y[i]]){
// 				printf("- %c\n",Y[i]);
				RE(j,X[i].length())
					O[X[i][j]]--;
				V.push_back(clear[i]);
			}
		}
		sort(V.begin(),V.end());
		RE(i,V.size())
			printf("%d",V[i]);
		printf("\n");
	}
	return 0;
}
