#include<iostream>
#include<string.h>
using namespace std;
int key[401];
int a[401][401];
int qu[401];
int point;
int type[401];
int sign[401];
int k,n,h,ans,s;
int need[250];
int have[250];
int max_type;
void openit(int d)
{
     sign[d]=1;
     key[type[d]]--;
     for(int i=1;i<=a[d][0];i++)
     key[a[d][i]]++;
     qu[point++]=d;
     need[type[d]]--;
     have[type[d]]--;
}
void back(int d)
{
     int i;
     sign[d]=0;
     key[type[d]]++;
     for(i=1;i<=a[d][0];i++)
     key[a[d][i]]--;
     point--;
     need[type[d]]++;
     have[type[d]]++;
}
int g(int x,int y)
{
    for(int i=1;i<=a[x][0];i++)
    if(a[x][i]==y) return 1;
    return 0;
}
int check(int d)
{
    int i;
    for(i=1;i<=max_type;i++)
    if(need[i]>have[i]) return 0;
    for(i=0;i<n;i++)
    {
                    if(!sign[i]&&have[type[i]]==0) return 0;
                    if(!sign[i]&&type[i]==type[d]&&need[type[i]]==1&&have[type[i]]==1&&g(i,type[i])) return 0;
    }
    return 1;
}
int dfs()
{
     int i,j;
     if(point==n) return 1;
     for(i=0;i<n;i++)
     {
                     if(key[type[i]]&&!sign[i]) 
                     {
                              openit(i);
                              if(!check(i)) {back(i);continue;}
                              if(dfs())return 1;
                              else back(i);
                     }
     }
     return 0;
}
int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int t;
    int i,j;
    scanf("%d",&t);
    for(i=1;i<=t;i++)
    {
               scanf("%d%d",&k,&n);
               s=n;
               memset(key,0,sizeof(key));
               memset(sign,0,sizeof(sign));
               memset(a,0,sizeof(a));
               memset(type,0,sizeof(type));
               memset(need,0,sizeof(need));
               memset(have,0,sizeof(have));
               max_type=0;
               for(j=1;j<=k;j++)
               {
                                scanf("%d",&key[0]);
                                key[key[0]]++;
                                have[key[0]]++;
                                if(max_type<key[0]) max_type=key[0];
               }
               for(j=0;j<n;j++)
               {
                               scanf("%d",&ans);
                               type[j]=ans;
                               need[ans]++;
                               scanf("%d",&a[j][0]);
                               for(h=1;h<=a[j][0];h++)
                               {
                                          scanf("%d",&a[j][h]);
                                          have[a[j][h]]++;
                                          if(max_type<a[j][h]) max_type=a[j][h];
                               }
               }
               point=0;
               //if(i==6) printf("%d %d\n",k,n);
               printf("Case #%d: ",i);
               for(j=1;j<=max_type;j++)
               if(need[j]>have[j])break;
               if(j!=(max_type+1)) {printf("IMPOSSIBLE\n");continue;}
               if(!dfs()) {printf("IMPOSSIBLE\n");continue;}
               for(j=0;j<point-1;j++)
               printf("%d ",1+qu[j]);
               printf("%d\n",1+qu[point-1]);
    }
    return 0;
}
