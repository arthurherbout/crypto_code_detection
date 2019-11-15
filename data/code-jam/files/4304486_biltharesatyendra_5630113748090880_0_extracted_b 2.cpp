#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


int a[1005][1005];

int vis[1005];
int solved=0;
int n;
int ans[555][555];
 int total;
int path[555];
int res;
void dfs(int m,vector<int> v[],int depth)
    {
    vis[m] = 1;
    if(solved)return;
  //  printf("dfs from %d\n",m);
    int i,j,k;
    
    path[depth] = m;
   
    for(i=1;i<=n;i++)
        ans[depth][i] = a[m][i];
    if(depth==n)
        {
      
        int valid = 1;
        for(i=1;i<=n;i++)
            {
            for(j=1;j<n;j++)
                if(ans[j][i] >= ans[j+1][i])
                {
               
                valid = 0;break;
            }
           if(valid==0)break;
            
        }
       // if(valid==0)printf("invalid column %d\n",i);
         for(i=1;i<=n;i++)
            {
            for(j=1;j<n;j++)
                if(ans[i][j] >= ans[i][j+1])
                {
                valid = 0;break;
            }
           
           if(valid==0)break;
            
        }
       // if(valid==0)printf("invalid row %d\n",i);
        if(valid)
            {
            int match = 0;
            int sol[555];
            set<int> s;
            for(i=1;i<=total;i++)sol[i] = vis[i];
            for(i=1;i<=n;i++)
                {
              //  printf("matching %d column\n",i);
                for(j=1;j<=total;j++)
                    {
                    if(sol[j]==0)
                        {
                        int valid = 1;
                        for(k=1;k<=n;k++)
                            if(ans[k][i]!=a[j][k])
                            {
                            valid = 0; break;
                        }
                        if(valid)
                            {
                           // printf("matched with col %d\n",j);
                            match++;
                            sol[j] = 1;
                            s.insert(i);
                            break;
                        }
                        //else res = i;
                    }
                }
            }
            if(match>n-1)printf("243298432432#$\n");
            if(match>=n-1){
                for(i=1;i<=n;i++)
                    if(s.find(i)==s.end()){res = i;break;}
              //  printf("print columnb %d\n",res);
                solved = 1;
                    
                /*
                           for(i=1;i<=n;i++)
                               {
                               for(j=1;j<=n;j++)
                                   printf("%d ",ans[i][j]);
                               printf("\n");
                           }*/
                return;
            }
        }
    }
    else {
        //vis[m] = 1;
        int l = v[m].size();
        for(i=0;i<l;i++)
            {
            k = v[m][i];
            if(vis[k]==0)
                {
               // printf("it has unvisited %d\n",k);
                dfs(k,v,depth+1);
            }
        }
       
    }
     vis[m] = 0;
}
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t,ca;
    scanf("%d",&t);
    
    for(ca=1; ca<=t; ca++)
        {
        
        scanf("%d",&n);
        int in[555];
        vector<int> v[555];
      
        int i,x;
        int j,k;
        total = 2*n-1;
        for(i=1;i<=total;i++)
            {
            for(j=1;j<=n;j++)
                scanf("%d",&a[i][j]);
        }
        for(i=1;i<=total;i++)
            {
            in[i] = 0;
            vis[i] = 0;
        }
        
        vector<int> g[555];
        for(i=1;i<=total;i++)
            {
            for(j=1;j<=total;j++)
                {
                int valid = 1;
                for(k=1;k<=n;k++)
                    if(a[i][k] >= a[j][k]){valid = 0; break;}
             //   printf("path of %d %d is %d\n",i,j,valid);
                 if(valid){ g[i].push_back(j);in[j]++;}
            }
           
        }
        solved=0;
        
        for(i=1;i<=total;i++)
            {
            
                for(j=1;j<=total;j++)vis[j] = 0;
                dfs(i,g,1);
                if(solved==1)break;
            
        }
        if(solved==0)printf("eroror #@$#@$#@$#@$\n");
        printf("Case #%d:",ca);
        for(i=1;i<=n;i++)
            printf(" %d",ans[i][res]);
        printf("\n");


        //printf("done with %d\n",x);
    }
    return 0;
}

