//#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstring>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <functional>
#include <bitset>
#define mp make_pair
#define pb push_back

#ifdef LOCAL
#define eprintf(...) fprintf(stderr,__VA_ARGS__)
#else
#define eprintf(...)
#endif

#define TIMESTAMP(x) eprintf("["#x"] Time : %.3lf s.\n", clock()*1.0/CLOCKS_PER_SEC)
#define TIMESTAMPf(x,...) eprintf("[" x "] Time : %.3lf s.\n", __VA_ARGS__, clock()*1.0/CLOCKS_PER_SEC)

#if ( _WIN32 || __WIN32__ )
    #define LLD "%I64d"
#else
    #define LLD "%lld"
#endif

using namespace std;

#define TASKNAME "C"
#define TASKMOD "small"

typedef long long ll;
typedef long double ld;

void PreCalc(){
}

const int MAXN = 20;
const int MAXM = 2100;

int n;

int from[MAXN];
int to[MAXN];
int Lf[MAXN];
int Rg[MAXN];
int d[MAXN];

int g[MAXN][MAXN];

void solve(){
	int m,p;
	scanf("%d %d %d",&n,&m,&p);
	for (int i = 0; i < m; i++){
		int a,b,c,d;
		scanf("%d %d %d %d",&a,&b,&c,&d);
		--a,--b;
		from[i] = a;
		to[i] = b;
		Lf[i] = c;
		Rg[i] = d;
	}

	vector<int> path(p);
	vector<bool> can(p);
	for (int i = 0; i < p; i++)
		scanf("%d",&path[i]),--path[i];

	for (int mask = 0; mask < (1<<m); mask++){
		for (int i = 0; i < n; i++)
			for (int j = 0;j < n; j++)
				g[i][j] = (1<<29) * (i != j);
		for (int i = 0; i < m; i++)
			g[from[i]][to[i]] = (mask & (1<<i))? Rg[i] : Lf[i];

		for (int k = 0; k < n; k++)
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					g[i][j] = min(g[i][j], g[i][k] + g[k][j]);

		for (int i = 0; i < p; i++){
			if (g[from[path[i]]][1] != ((mask & (1<<path[i]))? Rg[path[i]] : Lf[path[i]]) + g[to[path[i]]][1])
				break;
			can[i] = true;
		}		
	}

	for (int i = 0; i < p; i++)
		if (!can[i]) {
			eprintf("!! %d\n",i);
			printf("%d\n", path[i]+1);
			return;
		}

	printf("Looks Good To Me\n");
}


int main(){
  freopen(TASKNAME"-"TASKMOD".in","r",stdin);
  freopen(TASKNAME"-"TASKMOD".out","w",stdout);
	
  PreCalc();
  TIMESTAMP(PreCalc);	

  char buf[1000];
  int testNum;
  gets(buf);
  sscanf(buf,"%d",&testNum);

  for (int testId = 1; testId <= testNum; testId++){
  	if (testId <= 20 || testId >= testNum - 20 || testId % 10 == 0)
	  	TIMESTAMPf("Test %d",testId);
  	printf("Case #%d: ",testId);
  	solve();                        
  }
      
  TIMESTAMP(end);
  return 0;
}