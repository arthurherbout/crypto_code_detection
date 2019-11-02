#include<stdio.h>
#include<vector>
using namespace std;
vector<int> v;

#define myswap(a,b) { \
        int c = (a);    \
        (a) = (b);      \
        (b) = c;        \
    }

int a1, a2, b1, b2;

bool isAllOne()
{
	for(int i = 0; i < v.size(); i++)
		if(v[i] != 1) return 0;
	return 1;
}


bool isAllOne_1()
{
	for(int i = 0; i < v.size()-1; i++)
		if(v[i] != 1) return 0;
	return 1;
}

bool proc(int a, int b)
{
	v.clear();
	int t;

	if(a < b) myswap(a, b);

    while( 1 )
    {
		t = a/b;
		v.push_back(t);
        a = a % b;
		if( a == 0 )
			break;

		myswap(a, b);
	}

	if(isAllOne() && v.size() % 2 == 1) return 0;
	if(isAllOne_1() && v[v.size() - 1] > 1 && v.size()%2 == 1) return 1;
	if(v[0] == 1) return 0;
	return 1;

}

void solve()
{
	int c = 0;

	for(int i = a1; i <= a2; i++)
		for(int j = b1; j <= b2; j++)
			if(proc(i, j)) c++;

	printf("%d\n", c);
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("C-small-attempt0.in","r",stdin);
	freopen("C-small-attempt0.out","w",stdout);
#endif

	int cases;
	scanf("%d\n", &cases);
	for(int i = 1; i <= cases; i++)
	{
		printf("Case #%d: ", i);
		scanf("%d %d %d %d", &a1, &a2, &b1, &b2);
		solve();
	}


	return 0;

}
