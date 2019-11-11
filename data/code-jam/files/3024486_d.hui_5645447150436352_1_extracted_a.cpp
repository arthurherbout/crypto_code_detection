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
long long int sum[2000000];
long long int a[2000000];
long long int n,p,q,r,s,T,best,lo,hi,j,r1,r2;
long long int f(int lo,int hi)
{
    if(lo>hi)
        return 0;
    return sum[hi]-sum[lo-1];
}
int main()
{
	freopen("a-large.in","r",stdin);
	freopen("a-large.out","w",stdout);
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
            /*for(j=i;j<=n;j++)
                best=min(best,max(f(1,i-1),max(f(i,j),f(j+1,n))));*/
            lo=i;
            hi=n;
            while(lo<=hi)
            {
                j=(lo+hi)/2;
                r1=f(i,j);
                r2=f(j+1,n);
                best=min(best,max(f(1,i-1),max(r1,r2)));
                if(r1>r2)
                    hi=j-1;
                else
                    lo=j+1;
            }
            best=min(best,max(f(1,i-1),max(f(i,j),f(j+1,n))));
        }
        printf("%.9f\n",1.0*(sum[n]-best)/sum[n]);
    }
	fclose(stdin);
	fclose(stdout);
	return 0;
}
