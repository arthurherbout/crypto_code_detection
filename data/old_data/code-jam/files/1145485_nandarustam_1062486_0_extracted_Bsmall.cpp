#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <queue>
#include <complex>
#include <cmath>
#include <map>
#include <set>
using namespace std;

#define REP(i,n) for(int i = 0; i < n; ++i)
#define FOR(i,a,b) for(int i = a; i <= b; ++i)
#define FORd(i,a,b) for(int i = a; i >= b; --i)
#define mset(ar,x) memset(ar,x,sizeof(ar))
#define pub push_back
#define fr first
#define sc second
#define mp make_pair

typedef long long llong;
int tc,n,m;
char tmps[50];
string dict[105],lis;
vector<string> word[15];
set<char> huruf;

bool yea(const string &x, const string &g)
{
	int ln = g.length();
	REP(i,ln) if(g[i] != '*'){
		if(g[i] != x[i]) return 0;
	}
	return 1;
}

int main()
{
	scanf("%d",&tc);
	FOR(t,1,tc){
		scanf("%d %d",&n,&m);
		FOR(i,1,10) word[i].clear();
		REP(i,n){
			scanf("%s",tmps);
			dict[i] = tmps;
			word[(int)(dict[i].length())].pub(dict[i]);
		}
		
		printf("Case #%d:",t);
		REP(i,m){
			scanf("%s",tmps);
			lis = tmps;
			string ans;
			int point = -1;
			REP(j,n){
				int len = dict[j].length(), p = 0;
				string guess = "";
				bool valid[30];
				mset(valid,0);
				REP(k,(int)word[len].size()){
					REP(l,len){
						int x = lis.find(word[len][k][l]);
						valid[x] = 1;
					}
				}
				huruf.clear();
				REP(k,len) guess.pub('*'), huruf.insert(dict[j][k]);
				
				REP(k,(int)lis.length()) if(valid[k]){
					if(huruf.find(lis[k]) == huruf.end()) ++p;
					else{
						REP(l,(int)len) if(dict[j][l] == lis[k]) guess[l] = lis[k];
						int kount = 0;
						mset(valid,0);
						REP(l,(int)word[len].size()) if(yea(word[len][l], guess)){
							++kount;
							REP(z,len){
								int x = lis.find(word[len][l][z]);
								valid[x] = 1;
							}
						}
						if(kount == 1) break;
					}
				}
				if(point == -1){
					point = p;
					ans = dict[j];
				}
				if(p > point){
					point = p;
					ans = dict[j];
				}
			}
			printf(" %s",ans.c_str());
		}
		printf("\n");
	}
	
	return 0;
}



