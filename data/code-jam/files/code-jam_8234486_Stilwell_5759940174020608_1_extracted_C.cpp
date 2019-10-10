#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>
using namespace std;

int T,Case,n,i,j,k,ans,tmp;
int len[25],t[25];
long long h[25][1005];
int hh[25][1005];
char s[25][1005][15],ch;
bool wei;

bool read(char *s)
{
	for(ch=getchar();ch<'a'||ch>'z';ch=getchar());
	for(int i=1;ch>='a'&&ch<='z';++i)s[i]=ch,ch=getchar();
	if(ch=='\n'||ch=='\r')return true;
	return false;
}

map <long long ,int > hash;
int Hash[3005];

int cnt;

void dfs(int x)
{
	if(x>n)
	{
		int i,j,tmp=0;
		for(i=1;i<=k;++i)Hash[i]=0;
		for(i=1;i<=n;++i)
		for(j=1;j<=len[i];++j)
		if(!Hash[hh[i][j]])Hash[hh[i][j]]=t[i];
		else
		{
			if(Hash[hh[i][j]]!=t[i]&&Hash[hh[i][j]]!=3)
			Hash[hh[i][j]]=3,++tmp;
		}
		if(tmp<ans)ans=tmp;
		return;
	}
	t[x]=1;dfs(x+1);
	t[x]=2;dfs(x+1);
}

int main()
{
	freopen("1.in","r",stdin);
	freopen("1.out","w",stdout);
	scanf("%d",&T);
	for(;T;--T)
	{
		scanf("%d",&n);
		memset(s,0,sizeof(s));
		for(i=1;i<=n;++i)
		{
			for(len[i]=0;;)
			{
				++len[i];
				wei=read(s[i][len[i]]);
				h[i][len[i]]=0;
				for(j=1;s[i][len[i]][j]>='a'&&s[i][len[i]][j]<='z';++j)h[i][len[i]]=h[i][len[i]]*27+s[i][len[i]][j]-'a'+1;
				if(wei)break;
			}
		}
		t[1]=1;t[2]=2;ans=1000000000;
		hash.clear();k=0;
		for(i=1;i<=n;++i)
		for(j=1;j<=len[i];++j)
		{
			if(!hash[h[i][j]])hash[h[i][j]]=++k;
			hh[i][j]=hash[h[i][j]];
		}
		dfs(3);
		++Case;
		printf("Case #%d: %d\n",Case,ans);
		cerr << Case  << " " << k << endl;
	}
}
