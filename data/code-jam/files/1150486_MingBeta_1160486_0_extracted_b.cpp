#include<iostream>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<map>
#include<cstring>

using namespace std;

int a[505][505],s[505][505];
int r,c,d;

int sigma(int x,int y,int z,int t)
{
    return s[z][t]+s[x-1][y-1]-s[x-1][t]-s[z][y-1];
}

bool check(int x,int y,int l)
{
     int ll=l/2;
     int s1=sigma(x,y,x+l-1,y+ll-1)-a[x][y]-a[x+l-1][y];
     int s2=sigma(x,y+l-ll,x+l-1,y+l-1)-a[x][y+l-1]-a[x+l-1][y+l-1];
     //if (x==2 && y==2 && l==5) cout<<s1<<" "<<s2<<endl;
     if (s1!=s2) return 0;
     s1=sigma(x,y,x+ll-1,y+l-1)-a[x][y]-a[x][y+l-1];
     s2=sigma(x+l-ll,y,x+l-1,y+l-1)-a[x+l-1][y]-a[x+l-1][y+l-1];
     if (s1!=s2) return 0;
     //if (x==2 && y==2 && l==5) cout<<s1<<" "<<s2<<endl;
     return 1;
}

int main()
{
    freopen("B-small-attempt0.in","r",stdin);
    freopen("B-small-attempt0.out","w",stdout);
    int cs;
    scanf("%d",&cs);
    for (int k=1;k<=cs;k++){
        scanf("%d%d%d",&r,&c,&d);
        for (int i=1;i<=r;i++){
            char st[505];
            scanf("%s",&st);
            for (int j=0;j<c;j++) a[i][j+1]=st[j]-'0';
            }
        memset(s,0,sizeof(s));
        for (int i=1;i<=r;i++) 
            for (int j=1;j<=c;j++) s[i][j]=s[i-1][j]+a[i][j];
        for (int i=1;i<=r;i++)
            for (int j=1;j<=c;j++) s[i][j]=s[i][j-1]+s[i][j];
        int n=min(r,c);
        bool flag=false;
        int l;
        int ans;
        for (l=n;l>=3;l--)
            for (int i=1;i<=r-l+1;i++){ if (flag) break;
                for (int j=1;j<=c-l+1;j++) { if (flag) break;
                    if (check(i,j,l)){ flag=true; ans=l; break; }
                    } }
                    
                    
        printf("Case #%d: ",k);
        if (flag) printf("%d\n",ans); else printf("IMPOSSIBLE\n");
        }
}
