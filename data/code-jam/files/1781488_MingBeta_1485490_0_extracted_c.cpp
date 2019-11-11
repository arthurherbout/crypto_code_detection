#include<iostream>

using namespace std;

struct nodes{
       long long num,id;
       }a[105],b[105];
long long n,m,ans;

void dfs(int x,int y,long long z)
{
     if (z>ans) ans=z;
     if (x>=n || y>=m) return;
     if (a[x].id==b[y].id){
        if (a[x].num==b[y].num){
           dfs(x+1,y+1,z+a[x].num);
           } else
        if (a[x].num>b[y].num){
           a[x].num-=b[y].num;
           dfs(x,y+1,z+b[y].num);
           a[x].num+=b[y].num;
           } else
           {
           b[y].num-=a[x].num;
           dfs(x+1,y,z+a[x].num);
           b[y].num+=a[x].num;
           }
        }else
        {
             dfs(x+1,y,z);
             dfs(x,y+1,z);
        }
}

int main()
{
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small-attempt0.out","w",stdout);
    int test;
    cin>>test;
    for (int t=1;t<=test;t++){
        cin>>n>>m;
        for (int i=0;i<n;i++) cin>>a[i].num>>a[i].id;
        for (int i=0;i<m;i++) cin>>b[i].num>>b[i].id;
        ans=0;
        dfs(0,0,0);
        cout<<"Case #"<<t<<": "<<ans<<endl;
        }
}
