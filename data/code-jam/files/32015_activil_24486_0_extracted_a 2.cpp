#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

typedef vector<int> vi;
typedef map<int, int> mii;
typedef queue<int> qi;

#define FOR(i,f,t) for(int i=f; i<t; ++i)
#define REP(i,n) FOR(i,0,n)

int P, K, L;
vi fs;
vi keys;

int main(){
	FILE* f = fopen("a.in", "r");
	FILE* fo= fopen("a.out", "w+");
	int n;
	fscanf(f, "%d", &n);
	REP(i,n){
		fscanf(f, "%d %d %d", &P, &K, &L);
		fs.resize(L, 0);
		keys.resize(K, 0);
		REP(j, L){
			int t;
			fscanf(f, "%d", &t);
			fs[j]=t;
		}
		std::sort(fs.begin(), fs.end(), greater<int>());

		bool ok=true;
		int res=0;
		for (int ind=0, lev; (ind<L)&&fs[ind]; ++ind){
			lev = ind/K+1;
			if (lev > P){
				ok=false;
				break;
			}
			res+=fs[ind]*lev;
		}
		if (!ok) fprintf(fo, "Case #%d: Impossible\n", i+1);
		else fprintf(fo, "Case #%d: %d\n", i+1, res);
		
	}
	return 0;
}
