#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <map>
using namespace std;

#define REP(i,n) for(int i = 0, _n = n; i < _n; ++i)
#define FOR(i,a,b) for(int i = a, _b=b; i<= _b; ++i)
#define FORd(i,a,b) for(int i = a, _b=b; i>= _b; --i)
#define mset(ar,x) memset(ar,x,sizeof(ar))
#define pub push_back
#define fr first
#define sc second
#define mp make_pair

typedef long long llong;
typedef pair<char,int> Pci;
int tc,n;
vector<Pci> seq;
vector<int> b,o;

int main()
{
	scanf("%d",&tc);
	FOR(t,1,tc){
		scanf("%d",&n);
		char bot;
		int pos;
		seq.clear();
		o.clear();
		b.clear();
		REP(i,n){
			scanf("\n%c %d",&bot,&pos);
			seq.pub(mp(bot,pos));
			if(bot == 'O') o.pub(pos);
			else if(bot == 'B') b.pub(pos);
		}
		int bpos = 1, opos = 1, ib = 0, io = 0, i = 0, ans = 0;
		int sz = seq.size(), szb = b.size(), szo = o.size();
		ans = 0;
		while(i < sz){
			++ans;
			if(seq[i].fr == 'B' && ib < szb && bpos == b[ib]){
				++i;
				++ib;
				if(io < szo && o[io] > opos) ++opos;
				else if(io < szo && o[io] < opos) --opos;
			}else if(seq[i].fr == 'O' && io < szo && opos == o[io]){
				++i;
				++io;
				if(ib < szb && b[ib] > bpos) ++bpos;
				else if(ib < szb && b[ib] < bpos) --bpos;
			}else{
				if(io < szo && o[io] > opos) ++opos;
				else if(io < szo && o[io] < opos) --opos;
				
				if(ib < szb && b[ib] > bpos) ++bpos;
				else if(ib < szb && b[ib] < bpos) --bpos;
			}
		}
		
		printf("Case #%d: %d\n",t,ans);
	}
	
	return 0;
}


