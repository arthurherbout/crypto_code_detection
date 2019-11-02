#include<iostream>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
#include<windows.h>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

template<typename T>
int builtin_popcount(T x) { int ret = 0; while(x) { ++ret; x = ((x-1)&x); } return ret; }
struct Timer
{
	long long start, freq;
	Timer() { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
	double get() const { long long now; QueryPerformanceCounter((LARGE_INTEGER*)&now); return double(now - start) / double(freq); }
};
static Timer timer;

template<typename T, typename M, int MaxSize>
struct IndexedTree {
	int n;
	bool v[MaxSize*4]; 
	T t[MaxSize*4];
	M m;
	
	void mergenode(int i) {
		int left = (i<<1)+1;
		int right = (i<<1)+2; 
		if(v[right])
			t[i] = m.merge(t[left], t[right]);
		else
			t[i] = t[left];
	}

	void init(T a[], int size) {
		n = 1 << ((int)ceil(log((double)size) / log(2.0)));
		fill(v, v+n*2-1, false);
		for(int i=0;i<size;i++) {
			t[i+n-1] = a[i];
			v[i+n-1] = true;
		}
		for(int i=n-2;i>=0;i--) {
			v[i] = ( v[(i<<1)+1] || v[(i<<1)+2] );
			if(v[i])
				mergenode(i);
		}
	}
	
	void init(T init, int size) { 
		n = 1 << ((int)ceil(log((double)size) / log(2.0)));
		fill(v, v+n*2-1, false);
		for(int i=0;i<size;i++) {
			t[i+n-1] = init;
			v[i+n-1] = true;
		}
		for(int i=n-2;i>=0;i--) {
			v[i] = ( v[(i<<1)+1] || v[(i<<1)+2] );
			if(v[i])
				mergenode(i);
		}
	}
	
	T get(int start, int end) {
		int i1 = n-1+start;
		int i2 = n-1+end;
		if(i1 == i2)
			return t[i1]; 
			
		T left = t[i1];
		T right = t[i2];
		while(true) {
			int p1 = (i1-1)>>1;
			int p2 = (i2-1)>>1;
			if(p1 == p2)
				break;
			if((i1&1) == 1 && v[i1+1])
				left = m.merge(left, t[i1+1]);
			if((i2&1) == 0)
				right = m.merge(t[i2-1], right);
			i1 = p1;
			i2 = p2;
		}
		return m.merge(left, right);				
	}
	
	T get(int p) {
		return t[n-1+p];
	}
	
	void set(int i, T x) {
		i = n-1+i;
		t[i] = x;
		while(i > 0) {
			i = (i-1)>>1;
			mergenode(i);
		}
	}
};

struct Adder
{
	int merge(int a, int b) const
	{
		return a + b;
	}
};

int main()
{
	//freopen("input.txt", "r", stdin);
	freopen("c:\\gcj\\A-large.in", "r", stdin);
	freopen("c:\\gcj\\a-large.out", "w", stdout);
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		fprintf(stderr, "%d of %d (%.2lf sec) ..\r", cc+1, cases, timer.get());
		printf("Case #%d: ", cc+1);
		int n;
		cin >> n;
		vector<int> a(n), b(n), c(n);
		REP(i,n)
		{
			cin >> a[i] >> b[i] >> c[i];
		}
		vector<int> aSorted = a;
		sort(all(aSorted));
		aSorted.erase( unique(all(aSorted)), aSorted.end() );
		int ret = 0;
		REP(i,aSorted.sz)
		{
			vector<pair<int,int> > BC;
			REP(j,n) if(a[j] <= aSorted[i])
			{
				BC.pb(make_pair(b[j], c[j]));
			}
			sort(all(BC));

			IndexedTree<int,Adder,10001> tree;		
			tree.init(0, 10001);
			REP(j,BC.sz) 
			{
				tree.set(BC[j].second, tree.get(BC[j].second) + 1);
				int rem = 10000 - aSorted[i] - BC[j].first;
				if(rem < 0) break;
				int cand = tree.get(0, rem);
				ret = max(cand, ret);
			}
			//printf("after a = %d, %d\n", aSorted[i], ret);
		}
		printf("%d\n", ret);

	}
	fprintf(stderr, "Total elapsed time %.2lf sec\n", timer.get());
}
