#include<iostream>
#include<cstdio>
#include<cctype>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<string>
#include<list>
#include<deque>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<utility>
#include<sstream>
#include<cstring>
using namespace std;
 
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pii;

#define PB push_back
#define FORE(i,t) for(typeof(t.begin())i=t.begin();i!=t.end();++i)

char s[55555];

int main()
{
	int Z;
	scanf("%d",&Z);
	for(int z=1;z<=Z;++z)
	{
		printf("Case #%d: ",z);
		int k,t[22];
		scanf("%d%s",&k,s);
		for(int i=0;i<k;++i)
			t[i]=i;
		int best=999999999;
		do
		{
			int r=0;
			char last=0;
			for(int i=0;s[i];++i)
			{
				char act=s[i-i%k+t[i%k]];
				if(act!=last)
				{
					last=act;
					++r;
				}
			}
			best=min(best,r);
		}
		while(next_permutation(t,t+k));
		printf("%d\n",best);
	}
}
