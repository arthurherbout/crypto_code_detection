#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

typedef struct
{
	long long len;
	int id;
}Path;
Path P[1000006];
int Len[1000006];
int A[1005];

bool cmp(const Path &A,const Path &B)
{
	if(A.len==B.len) return A.id>B.id;
	return A.len>B.len;
}

int main()
{
	freopen("B-large.in","r",stdin);
	freopen("B.out","w",stdout);
	int _,cases=1;
	scanf("%d",&_);
	while(_--)
	{
		int N,L,C;
		long long t;
		scanf("%d%I64d%d%d",&L,&t,&N,&C);
		for(int i=0;i<C;i++) scanf("%d",&A[i]);
		bool done=false;
		long long res=0LL;
		for(int k=0;k<=N&&!done;k++)
		{
			for(int i=0;i<C;i++)
			{
				if(k*C+i>=N) { done=true; break; }
				res+=A[i]*2LL;
				P[k*C+i].len=A[i]*2;
				P[k*C+i].id=k*C+i;
				Len[k*C+i]=A[i]*2;
			}
		}
		long long sum=0;
		int idx=-1;
		for(int i=0;i<N&&idx==-1;i++)
		{
			sum+=P[i].len;
			if(sum>=t)
			{
				idx=i;
				P[i].len=sum-t;
			}
			else P[i].len=0;
		}
		sort(P,P+N,cmp);
		for(int i=0;i<L;i++)
		{
			if(P[i].len<=0) break;
			res-=P[i].len/2;
		}
		printf("Case #%d: %I64d\n",cases++,res);
	}
	return 0;
}