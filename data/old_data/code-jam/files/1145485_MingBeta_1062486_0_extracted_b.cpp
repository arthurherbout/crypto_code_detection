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
       int id,ip,l;
       } c[10005];
int n,m,ans[10005],tot;
char a[10005][20],b[108][30];

bool cmp(const nodes &x,const nodes &y)
{
     return x.ip>y.ip;
}

bool icmp(const nodes &x,const nodes &y)
{
     return x.l<y.l;
}

void doit(int y,int x,int l,int r,int z)
{
     if (l==r){
        //cout<<l<<" "<<c[l].id<<" "<<z<<endl;
        ans[c[l].id]=z;
        return;
        }
     for (int i=l;i<=r;i++){
         c[i].ip=0;
         int len=strlen(a[c[i].id]);
         for (int j=0;j<c[i].l;j++) 
             if (a[c[i].id][j]==b[y][x]) c[i].ip+=1<<j;
         }
     sort(c+1+l,c+1+r,cmp);
     int now=z;
     if (c[l].ip==c[r].ip) doit(y,x+1,l,r,z); else{
        int t=l;
        for (int i=l;i<=r;i++)
            if (c[i+1].ip!=c[t].ip || i==r){
               if (c[i].ip==0) now++;
               doit(y,x+1,t,i,now);
               t=i+1;
               }
        }
}

int main()
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-small-attempt0.out","w",stdout);
    int cs,i,j;
    cin>>cs;
    for (int loop=1;loop<=cs;loop++){
        cin>>n>>m;
        for (i=1;i<=n;i++) cin>>a[i];
        for (i=1;i<=m;i++) cin>>b[i];
        sort(c+1,c+n+1,icmp);
        cout<<"Case #"<<loop<<":";
        for (i=1;i<=m;i++){
            memset(ans,0,sizeof(ans));
            tot=0;
            int tmp=1;
            for (j=1;j<=n;j++){
                c[j].id=j;
                c[j].ip=0;
                c[j].l=strlen(a[j]);
                }
            for (j=1;j<=n;j++)
                if (c[j+1].l!=c[tmp].l || j==n){
                    doit(i,0,tmp,j,0);
                    tmp=j+1;
                    }
            //for (j=1;j<=n;j++) cout<<ans[j]<<" ";
            int t=0,max=-1;
            for (j=1;j<=n;j++) 
                if (ans[j]>max){
                   max=ans[j];
                   t=j;
                   }
            cout<<" "<<a[t];
            }
        cout<<endl;
        }
}
