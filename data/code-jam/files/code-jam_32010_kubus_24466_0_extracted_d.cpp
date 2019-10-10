#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
using namespace std;

#define CLEAR(t) memset((t),0,sizeof(t))
#define FOR(i,a,b) for(__typeof(a)i=(a);i<=(b);++i)
#define FORD(i,a,b) for(__typeof(a)i=(a);i>=(b);--i)
#define REP(i,n) for(__typeof(n)i=0;i<(n);++i)
#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
#define PB(x) push_back(x)
#define MP(x,y) make_pair(x,y)

vector<string> split(const string& s, const string& delim=" ")
{ vector<string> res; string t; for(unsigned int i=0;i<s.length();i++) { if(delim.find(s[i])!=string::npos) { if(!t.empty()) { res.PB(t); t=""; } } else t+=s[i]; } if(!t.empty()) res.PB(t); return res; }
vector<int> splitint(const string& s, const string& delim=" ") { vector<string> vs=split(s); vector<int> res; for(unsigned int i=0;i<vs.size();i++) res.PB(atoi(vs[i].c_str())); return res; }

int n,k,p;
vector<vector<int> > s;
int r[1024][2];

void ms(int x)
{
  if(x&(1<<(p-1))) { s[x].PB(1+2*(x^(1<<(p-1)))); return; } 
  REP(j,p)if(x&(1<<j)) s[x].PB(1+2*(x^(1<<j)));
}

void _case(int casenum)
{
  scanf("%d%d%d",&n,&k,&p);
  s.clear();
  s.resize(1<<p);
  REP(i,1<<p) ms(i);
  int b=(1<<k)-1;
  CLEAR(r);
  r[b][0]=1;
//  REP(i,1<<p){printf("%d:",i);REP(j,s[i].size())printf(" %d",s[i][j]);printf("\n");}
  FOR(t,1,n-k)
  {
    REP(i,1<<p) r[i][t&1]=0;
    REP(i,1<<p)REP(j,s[i].size()) r[s[i][j]][t&1]+=r[i][(t-1)&1];
    REP(i,1<<p) r[i][t&1]%=30031;
  }
  printf("Case #%d: %d\n",casenum,r[b][(n-k)&1]);
}

int main()
{
  int n;
  scanf("%d ",&n);
  FOR(i,1,n) _case(i);
  return 0;
}
