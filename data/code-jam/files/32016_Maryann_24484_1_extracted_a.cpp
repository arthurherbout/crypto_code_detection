#include<stdio.h>
#include<algorithm>
#define ll long long
using namespace std;
ll ans;
int n,a[999],b[999];
int main()
{
    int _;
    scanf("%d",&_);
    for(int t=1; t<=_; t++)
    {
        scanf("%d",&n);
        for(int i=0; i<n; i++)
        scanf("%d",&a[i]);
        for(int i=0; i<n; i++)
        scanf("%d",&b[i]);
        sort(a,a+n);
        sort(b,b+n);
        ans=0;
        for(int i=0; i<n; i++)
        ans+=((ll)a[i])*b[n-1-i];
        printf("Case #%d: %I64d\n",t,ans);
    }
    return 0;
}
