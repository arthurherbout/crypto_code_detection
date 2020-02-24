#include <cstdio>
#include <map>
#include <vector>
using namespace std;
int a[555],n;
void solve(int i,int u,int s,map<int,vector<int> >&v)
{
  if(i<0){
    if(s)v[s].push_back(u);
    return;
  }
  solve(i-1,u<<1,s,v);
  solve(i-1,(u<<1)|1,s+a[i],v);
}
int main(void)
{
  int t;
  scanf("%d",&t);
  for(int T=1;T<=t;T++){
    map<int,vector<int> > v;
    printf("Case #%d:\n",T);
    scanf("%d",&n);
    for(int i=0;i<n;i++){
      scanf("%d",a+i);
    }
    solve(n-1,0,0,v);
    for(auto it=v.begin();it!=v.end();++it){
      vector<int>&p=it->second;
      for(int i=0;i<int(p.size());i++){
        for(int j=i+1;j<int(p.size());j++){
          if(!(p[i]&p[j])){
            //printf("%d %d %d\n",it->first,p[i],p[j]);
            {bool o=false;
              for(int ii=0;ii<n;ii++){
                if(p[i]>>ii&1){
                  if(o)putchar(' ');
                  o=true;
                  printf("%d",a[ii]);
                }
              }
              puts("");
            }
            {bool o=false;
              for(int ii=0;ii<n;ii++){
                if(p[j]>>ii&1){
                  if(o)putchar(' ');
                  o=true;
                  printf("%d",a[ii]);
                }
              }
              puts("");
            }
            goto NEXT;
          }
        }
      }
    }
    puts("Impossible");
  NEXT:;
  }
  return 0;
}
