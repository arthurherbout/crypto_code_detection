#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;
int sum[2000];
int a[2000];
int n,p,q,r,s,T,best;
int f(int lo,int hi)
{
    if(lo>hi)
        return 0;
    return sum[hi]-sum[lo-1];
}
int main()
{
	freopen("a-small.in","r",stdin);
	freopen("a-small.out","w",stdout);
	cin>>T;
	for(int zz=1;zz<=T;zz++)
    {
        printf("Case #%d: ",zz);
        cin>>n>>p>>q>>r>>s;
        best=0;
        memset(sum,0,sizeof(sum));
        for(int i=1;i<=n;i++)
        {
            a[i]=((i-1)*p+q)%r+s;
            sum[i]=sum[i-1]+a[i];
        }
        best=sum[n];
        for(int i=1;i<=n;i++)
        {
            for(int j=i;j<=n;j++)
                best=min(best,max(f(1,i-1),max(f(i,j),f(j+1,n))));
        }
        printf("%.9f\n",1.0*(sum[n]-best)/sum[n]);
    }
	fclose(stdin);
	fclose(stdout);
	return 0;
}
