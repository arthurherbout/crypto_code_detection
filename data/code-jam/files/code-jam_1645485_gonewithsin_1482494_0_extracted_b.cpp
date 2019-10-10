#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

int n,t,counts,stars,hits;
int a[1005],b[1005];
int v[1005];

void qsort(int s,int t)
{
    int i,j,mid;
    i=s;j=t;mid=b[(i+j)/2];
    while(i<=j)
    {
        while(b[i]>mid)i++;
        while(b[j]<mid)j--;
        if(i<=j)
        {
            swap(a[i],a[j]);
            swap(b[i],b[j]);
            //swap(c[i],c[j]);
            i++;j--;
        }
    }
    if(i<t)qsort(i,t);
    if(s<j)qsort(s,j);
}



int main()
{
    //
    freopen("B-small-attempt0.in","r",stdin);
    //
    freopen("out.out","w",stdout);
    scanf("%d",&t);
    for(int k=1;k<=t;k++)
    {
        scanf("%d",&n);
        counts=stars=hits=0;
        for(int i=1;i<=n;i++)
        {
            scanf("%d%d",&a[i],&b[i]);
            v[i]=0;
        }
        qsort(1,n);
        bool f1,f2;
        for(int j=1;j<=2*n;j++)
        {
            f1=f2=0;
            if(hits==n)break;
            for(int i=1;i<=n;i++)
                if(v[i]<2&&b[i]<=stars)
                {
                    counts++;
                    hits++;
                    stars+=2-v[i];
                    v[i]=2;
                    f1=1;
                    break;
                }
            if(f1)continue;

            for(int i=1;i<=n;i++)
                if(v[i]==0&&a[i]<=stars)
                {
                    counts++;
                    stars+=1;
                    v[i]=1;
                    f2=1;
                    break;
                }
            if(!f2)break;
        }
        printf("Case #%d: ",k);
        if(hits!=n)printf("Too Bad\n");
            else printf("%d\n",counts);
    }
    return 0;
}
