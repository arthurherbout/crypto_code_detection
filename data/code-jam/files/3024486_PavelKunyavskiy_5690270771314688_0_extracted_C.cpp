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

const int N = 15;

bool can[(1<<N)+10];
bool ncan[(1<<N)+10];
map<int, int> ids;
int getId(int id) {
	if (ids.find(id) == ids.end()){
		int tmp = ids.size();
		ids[id] = tmp;
	}
	return ids[id];
}

void solve(){
	ids.clear();
	memset(can, 1, sizeof(can));

	int n;
	scanf("%d",&n);

	for (int i = 0; i < n; i++) {
		memset(ncan, 0, sizeof(ncan));

		char c;
		int id;
		scanf(" %c %d",&c,&id);
		--id;
		if (id != -1) id = getId(id);

		for (int j = 0; j < N; j++)
			if (id == -1 || id == j) {
				int cur = c == 'E' ? 0 : 1;
				for (int i = 0; i < (1<<n); i++)
					if (can[i] && ((i >> j) & 1) == cur)
						ncan[i ^ (1<<j)] = true;
			}
		memcpy(can, ncan, sizeof(ncan));
	}
	int ans = N + 1;
	for (int i = 0; i < (1<<N); i++)
		if (can[i])
			ans = min(ans, __builtin_popcount(i));
	if (ans == N+1)
		puts("CRIME TIME");
	else
		printf("%d\n", ans);

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