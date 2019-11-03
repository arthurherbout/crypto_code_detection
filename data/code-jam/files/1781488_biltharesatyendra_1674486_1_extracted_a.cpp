#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<map>
#include<string>
#include<ctype.h>

using namespace std;
typedef unsigned long long int64;
queue<int> q;
/*
int bfs(int m,vector<int> v[])
{//printf("bfs at %d\n",m);
    queue<int> q;
    int cur;
    fl[m]=1;
    q.push(m);
    while(!q.empty())
    {
                     cur=q.front();
                     q.pop();
                     int l=v[cur].size();
                     int i;
                     int x;
                     for(i=0;i<l;i++){
                                      x=v[cur][i];
                                     // printf("connected to %d\n",x);
                     if(fl[x])if(x-m)return 1;
                     else {
                          fl[x]=1;
                          q.push(x);
                          }
                          }
                          
                          }
                          return 0;
                          }
                          */
                         int dfs(int m,vector<int> v[],int fl[])
                          {
                                     fl[m]=1;
                                     int valid,x;
                                     int l=v[m].size();
                                     int i;
                                     for(i=0;i<l;i++)
                                     {
                                                  int   x=v[m][i];
                                                     if(fl[x]==1)return 1;
                                                     q.push(x);
                                                     valid=dfs(x,v,fl);
                                                     if(valid)return 1;
                                                     }
                                     //fl[m]=0;
                             return 0;        
                          }
int main()
{
    int t;
    scanf("%d",&t);
    int ca;
    for(ca=1;ca<=t;ca++)
    {
                        int n;
                        vector<int> v[1005];
                        scanf("%d",&n);
                        int i;
                        int m;
                          int j,x;
                        for(i=1;i<=n;i++)
                        {
                                        scanf("%d",&m);
                                      for(j=0;j<m;j++)
                                      {
                                                      scanf("%d",&x);
                                                      v[i].push_back(x);
                                                      }
                                        }
                                        int fl[1005];
                                        for(i=1;i<=n;i++)fl[i]=0;
                                        int valid=0;
                                        for(i=1;i<=n;i++){
                                        if(fl[i]==0){q.push(i);
                                                     valid=dfs(i,v,fl);}
                                        if(valid)break;
                                        while(!q.empty())
                                        {
                                                         x=q.front();
                                                         fl[x]=0;
                                                         q.pop();
                                                         }
                                        }
                                        if(valid)
                                        printf("Case #%d: Yes\n",ca);
                                        else
                                        printf("Case #%d: No\n",ca);
                        }
    return 0;
}
