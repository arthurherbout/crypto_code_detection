#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <set>
#include <map>
#include <complex>
#include <queue>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string.h>
#include <unordered_set>
#include <unordered_map>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <cassert>

using namespace std;

typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef pair<int, int> ipair;
typedef vector<int> VI;
typedef vector<string> VS;
typedef vector<double> VD;
#define SIZE(A) ((int)(A.size()))
#define LENGTH(A) ((int)(A.length()))
#define MP(A, B) make_pair(A,B)
const double pi = acos(-1.0);
#define FOR(i, a, b) for(int i=(a);i<(b);++i)
#define REP(i, a) for(int i=0;i<(a);++i)
#define ALL(a) (a).begin(),(a).end()

template<class T>
T sqr(const T& x) { return x * x; }

template<class T>
T lowbit(const T& x) { return (x ^ (x - 1)) & x; }

template<class T>
int countbit(const T& n) { return (n == 0) ? 0 : (1 + countbit(n & (n - 1))); }

template<class T>
void ckmin(T& a, const T& b) { if (b < a) a = b; }

template<class T>
void ckmax(T& a, const T& b) { if (b > a) a = b; }

const int maxsize=128;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};

int size_x,size_y,md;
char a[maxsize][maxsize];
int n,m;
int px1[maxsize],py1[maxsize];
int px2[maxsize],py2[maxsize];
int d[maxsize][maxsize];
ipair q[maxsize*maxsize];
bool o[maxsize][maxsize][maxsize];

bool visited[2<<20];
vector<ipair> ret;

void dfs(int set,vector<ipair> current)
{
	if (SIZE(current)>SIZE(ret)) ret=current;
	if (visited[set]) return;
	visited[set]=true;
	vector<pair<int,ipair>> next_states;
	REP(sid,n) if (set&(1<<sid))
	{
		memset(d,255,sizeof(d));
		d[px1[sid]][py1[sid]]=0;
		int sizeq=0;
		q[sizeq++]=MP(px1[sid],py1[sid]);
		REP(cl,sizeq)
		{
			int x0=q[cl].first,y0=q[cl].second;
			bool w=false;
			REP(tid,m) if ((set&(1<<(n+tid))) && o[tid][x0][y0])
			{
				w=true;
				int new_set=(set^(1<<(n+tid))^(1<<sid));
				next_states.push_back(MP(new_set,MP(sid,tid)));
			}
			if (w) continue;
			if (d[x0][y0]>=md) continue;
			REP(direction,4)
			{
				int x2=x0+dx[direction];
				int y2=y0+dy[direction];
				if (x2>=0 && x2<size_x && y2>=0 && y2<size_y && d[x2][y2]<0 && a[x2][y2]!='#') 
				{
					d[x2][y2]=d[x0][y0]+1;
					q[sizeq++]=MP(x2,y2);
				}
			}
		}
	}
	for (auto state:next_states)
	{
		int new_set=state.first;
		if (visited[new_set]) continue;
		auto t=current;
		t.push_back(state.second);
		dfs(new_set,t);
	}
}
vector<ipair> solve()
{
	n=m=0;
	REP(x,size_x) REP(y,size_y) if (a[x][y]=='S') px1[n]=x,py1[n++]=y;
	REP(x,size_x) REP(y,size_y) if (a[x][y]=='T') px2[m]=x,py2[m++]=y;
	memset(o,false,sizeof(o));
	REP(tid,m) REP(d,4) 
		for (int x0=px2[tid],y0=py2[tid];x0>=0 && x0<size_x && y0>=0 && y0<size_y && a[x0][y0]!='#';x0+=dx[d],y0+=dy[d])
			o[tid][x0][y0]=true;
	memset(visited,false,sizeof(visited));
	vector<ipair> current;
	ret.clear();
	dfs((1<<(n+m))-1,current);
	return ret;
}
int main()
{
  //freopen("D.in","r",stdin);
  //freopen("D-small-attempt0.in","r",stdin); freopen("D-small-attempt0.out","w",stdout);
  //freopen("D-small-attempt1.in","r",stdin); freopen("D-small-attempt1.out","w",stdout);
  freopen("D-small-attempt2.in","r",stdin); freopen("D-small-attempt2.out","w",stdout);
  //freopen("D-large.in","r",stdin); freopen("D-large.out","w",stdout);

  // std::ios_base::sync_with_stdio(false);
  int ntestcase;
  cin >> ntestcase;
  for (int case_id = 1; case_id <= ntestcase; ++case_id) {
	  cin>>size_y>>size_x>>md;
	  REP(x,size_x) cin>>a[x];
	  printf("Case #%d: ", case_id);
      vector<ipair> ret=solve();
	  printf("%d\n",SIZE(ret));
	  for (auto d:ret) printf("%d %d\n",d.first+1,d.second+1);
	  fflush(stdout);
  }

  return 0;
}