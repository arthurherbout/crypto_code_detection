#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <cmath>
using namespace std;

int a[10000];

int p, q;

bool b[10000];

int doIt(int x)
{
	int ans = 0;
	
	b[x] = 1;
	
	for (int i = x - 1; i>0 && b[i]==0; i--, ans++)
	{
		int df=0;
	}
	for (int i = x + 1; i<=p && b[i]==0; i++, ans++)
	{
		int sdf=0;
	}
	
	return ans;	
}

int main ()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	
	int n;
	cin >> n;
	
	for (int k=0; k<n; k++)
	{
		cin >> p >> q;
		
		for (int i=0; i<q; i++)
		{
			cin >> a[i];
		}
		
		sort(a, a + q);
		
		int ans = 2000000000;
		
		do
		{
			for (int i=1; i<=p; i++)
			{
				b[i] = 0;				
			}
			
			int d = 0;
			
			for (int i=0; i<q; i++)
			{
				d += doIt(a[i]);
			}
			
			ans = min(ans, d);
			
		}while (next_permutation(a, a + q));
		
		cout << "Case #" << k + 1 << ": " << ans << endl;
	}
		
    return 0;
}
