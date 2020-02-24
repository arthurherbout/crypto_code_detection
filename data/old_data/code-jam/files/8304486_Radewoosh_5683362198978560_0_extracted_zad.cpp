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

int lower_limit=-1;
int upper_limit=1e8;

const int nax=207;
const long long mod=1000*1000*1000+7;
const long long inf=mod*mod;

int r, c;
int n;
long long d;

long long tab[nax][nax];

long long maxi[nax][nax];

long long wyn;

void test_case(int tid)
{
	debug() << "running test case #" << tid;
	
	//wczytaj
	scanf("%d%d%d%lld", &r, &c, &n, &d);
	
	for (int i=1; i<=r; i++)
	{
		for (int j=1; j<=c; j++)
		{
			maxi[i][j]=inf;
			tab[i][j]=0;
		}
	}
	for (int i=1; i<=n; i++)
	{
		int x, y;
		scanf("%d%d", &x, &y);
		scanf("%lld", &tab[x][y]);
		for (int j=1; j<=r; j++)
			for (int l=1; l<=c; l++)
				maxi[j][l]=min(maxi[j][l], tab[x][y]+d*(abs(j-x)+abs(l-y)));
	}
	
	if (tid<lower_limit || tid>upper_limit)
		return;
	printf("Case #%d: ", tid);
	
	int ok=1;
	wyn=0;
	for (int i=1; i<=r; i++)
	{
		for (int j=1; j<=c; j++)
		{
			if (tab[i][j] && tab[i][j]!=maxi[i][j])
				ok=0;
			wyn+=maxi[i][j];
			wyn%=mod;
		}
	}
	
	if (!ok)
		printf("IMPOSSIBLE\n");
	else
		printf("%lld\n", wyn);
	
	//zarzaj
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
