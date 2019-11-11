#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
vector <int> t;
int check(int A, int i, int &k)
{
	int res=0; k=A;
	while(k!=1 && k<=t[i])
	{
		k*=2; k--; res++;
	}
	return res;
}
int main()
{
    int d, A, n, x;
    scanf("%d\n", &d);
    for(int k=1; k<=d; k++)
    {
		
		scanf("%d %d\n", &A, &n);
		while(t.size()>0) t.pop_back();
		for(int i=0; i<n; i++)
		{
			scanf("%d", &x);
			t.push_back(x);
		}
		sort(t.begin(), t.end());
		int res=0;
		for(int i=0; i<t.size(); i++)
		{
			if(A>t[i]) A+=t[i];
			else
			{
				if(2*A-1 > t[i])
				{
					res++;
					A = 2*A-1+t[i];
				}
				else
				{
					int p, tmp;
					tmp = check(A, i, p);
					if(p>1 && tmp<t.size()-i)
					{
						A = p+t[i];
						res+=tmp;
					}
					else 
					{
						res+=t.size()-i;
						break;
					}
				}
			}
		}
		printf("Case #%d: %d\n", k, res);
    }
    return 0;
}
