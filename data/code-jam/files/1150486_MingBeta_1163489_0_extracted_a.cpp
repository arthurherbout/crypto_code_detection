#include<iostream>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<map>
#include<cstring>

using namespace std;

struct nodes{
       double w,e,b;
       } a[1005];
double l,s,r,t;
int n;

bool cmp1(const nodes &aa,const nodes &bb)
{
     return aa.b<bb.b;
}

bool cmp2(const nodes &aa,const nodes &bb)
{
     return aa.w<bb.w;
}

int main()
{
    freopen("A-small-attempt0.in","r",stdin);
    freopen("A-small-attempt0.out","w",stdout);
    int cs;
    scanf("%d",&cs);
    for (int k=1;k<=cs;k++){
        scanf("%lf%lf%lf%lf%d",&l,&s,&r,&t,&n);
        for (int i=1;i<=n;i++)
            scanf("%lf%lf%lf",&a[i].b,&a[i].e,&a[i].w);
        sort(a+1,a+n+1,cmp1);
        int m=n;
        if (a[1].b>0){ m++; a[m].b=0; a[m].e=a[1].b; a[m].w=0; }
        if (a[n].e<l){ m++; a[m].b=a[n].e; a[m].e=l; a[m].w=0; }
        for (int i=1;i<n;i++)
            if (a[i].e<a[i+1].b){
               m++;
               a[m].b=a[i].e;
               a[m].e=a[i+1].b;
               a[m].w=0;
               }
        double ans=0;
        sort(a+1,a+m+1,cmp2);
        for (int i=1;i<=m;i++){
            double len=a[i].e-a[i].b;
            double it=len/(a[i].w+r);
            if (t>=it){
               t-=it;
               ans+=it;
               } else {
               len-=t*(a[i].w+r);
               ans+=t;
               t=0;
               it=len/(a[i].w+s);
               ans+=it;
               }
            }
        printf("Case #%d: %.6lf\n",k,ans);
        }
}
            
