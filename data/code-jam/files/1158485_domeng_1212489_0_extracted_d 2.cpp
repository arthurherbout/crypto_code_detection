#include <cstdio>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>
using namespace std;
vector<int> tr;
char str[255];
int main()
{
	int T;	scanf("%d",&T);
	for (int kase=1;kase<=T;++kase)
	{
		tr.clear();
		scanf("%s",str);
		int len = 0;
		for (int i=0;str[i];++i)
		{
			if (str[i]=='?')
				tr.push_back(i);
			len = i;
		}
		for (int v=0;v<(1<<tr.size());++v)
		{
			for (int q=0;q<tr.size();++q) str[ tr[q] ] = ((v>>q)&1)+'0';
			long long sq =0;
			for (int q=0;str[q];++q) sq += (1LL<<(len-q)) * (str[q]-'0');
			long long root = max( static_cast<long long>(sqrt(double(sq)))-2,0LL);

			//printf("try %d > str %s > sq %lld > root %lld\n",v,str,sq,root);
			for (;root*root<sq;++root);
			if (root*root == sq) 
			{
				printf("Case #%d: %s\n",kase,str);
				break;
			}
		}
	}
	return 0;
}
