//PRZEMYSL ASSERTY

//SPRAWDZ CORNER CASE'Y, MINIMALNE I MAKSYMALNE WEJŚCIE I WYJŚCIE

//MODULO = 1

//while (clock()<=69*CLOCKS_PER_SEC)

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "

#define shandom_ruffle random_shuffle

using piii= pair <int, pair <int,int> >;

int lower_limit=-1;
int upper_limit=1e8;

const int nax=2007;

int n;

int sko[nax];
int gdz[nax];
int zacz[nax];
int kon[nax];
int trwa[nax];

int oj[nax];

vector <int> wcho[nax];
vector <int> wych[nax];

vector < piii > merdz;

int wyn;

int fin(int v)
{
	if (v!=oj[v])
		oj[v]=fin(oj[v]);
	return oj[v];
}

void uni(int v, int u)
{
	v=fin(v);
	u=fin(u);
	oj[v]=u;
}

int od_do(int v, int u)
{
	v%=24;
	u%=24;
	while(u<v)
		u+=24;
	return u-v;
}

/*void dwie_opcje(piii a, piii b)
{
	if (b<a)
		swap(a, b);
	wyn+=a.first;
	b.first-=a.first;
	uni(a.second.first, a.second.second);
	merdz.push_back(b);
}*/

void test_case(int tid)
{
	debug() << "running test case #" << tid;
	
	//wczytaj
	
	scanf("%d", &n);
	for (int i=1; i<=2*n; i++)
	{
		sko[i]=(i+1)/2;
		scanf("%d", &gdz[i]);
		scanf("%d%d", &zacz[i], &trwa[i]);
		
		oj[i]=i;
	}
	
	if (tid<lower_limit || tid>upper_limit)
		return;
	printf("Case #%d: ", tid);
	
	//zarzaj
	
	wyn=0;
	merdz.clear();
	for (int i=1; i<=n; i++)
	{
		wcho[i].clear();
		wych[i].clear();
	}
	
	for (int i=1; i<=2*n; i++)
	{
		wych[sko[i]].push_back(i);
		wcho[gdz[i]].push_back(i);
		wyn+=trwa[i];
		kon[i]=(zacz[i]+trwa[i])%24;
	}
	for (int i=1; i<=n; i++)
	{
		if (i==1)
		{
			piii a={0, {wcho[i][0], wych[i][1]}};
			piii b={0, {wcho[i][1], wych[i][0]}};
			
			piii c={0, {wcho[i][0], wych[i][0]}};
			piii d={0, {wcho[i][1], wych[i][1]}};
			
			int raz=od_do(0, zacz[wych[i][1]])+od_do(kon[wcho[i][1]], zacz[wych[i][0]]);
			int dwa=od_do(0, zacz[wych[i][0]])+od_do(kon[wcho[i][0]], zacz[wych[i][1]]);
			
			int trz=od_do(0, zacz[wych[i][0]])+od_do(kon[wcho[i][1]], zacz[wych[i][1]]);
			int czt=od_do(0, zacz[wych[i][1]])+od_do(kon[wcho[i][0]], zacz[wych[i][0]]);
			
			a.first=min(raz, dwa);
			c.first=min(trz, czt);
			
			if (a.first+b.first>c.first+d.first)
			{
				swap(a, c);
				swap(b, d);
			}
			
			wyn+=a.first+b.first;
			uni(a.second.first, a.second.second);
			uni(b.second.first, b.second.second);
			
			int ko=c.first+d.first-a.first-b.first;
			merdz.push_back({ko, {a.second.first, b.second.first}});
		}
		else
		{
			piii a={od_do(kon[wcho[i][0]], zacz[wych[i][1]]), {wcho[i][0], wych[i][1]}};
			piii b={od_do(kon[wcho[i][1]], zacz[wych[i][0]]), {wcho[i][1], wych[i][0]}};
			
			piii c={od_do(kon[wcho[i][0]], zacz[wych[i][0]]), {wcho[i][0], wych[i][0]}};
			piii d={od_do(kon[wcho[i][1]], zacz[wych[i][1]]), {wcho[i][1], wych[i][1]}};
			
			if (a.first+b.first>c.first+d.first)
			{
				swap(a, c);
				swap(b, d);
			}
			
			wyn+=a.first+b.first;
			uni(a.second.first, a.second.second);
			uni(b.second.first, b.second.second);
			
			int ko=c.first+d.first-a.first-b.first;
			merdz.push_back({ko, {a.second.first, b.second.first}});
		}
	}
	
	sort(merdz.begin(), merdz.end());
	
	for (auto i : merdz)
	{
		if (fin(i.second.first)!=fin(i.second.second))
		{
			wyn+=i.first;
			uni(i.second.first, i.second.second);
		}
	}
	
	printf("%d\n", wyn);
}

int main(int argc, char* argv[])
{
	if (argc==3)
	{
		lower_limit=atoi(argv[1]);
		upper_limit=atoi(argv[2]);
	}
	int t;
	scanf("%d", &t);
	for (int i=1; i<=t; i++)
		test_case(i);
	return 0;
}
