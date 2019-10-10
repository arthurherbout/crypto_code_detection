#include<iostream>

using namespace std;

int cnt[2000005];

int f(int x)
{
    int y=1;
    while (x/y>=10) y*=10;
    return y;
}

int main()
{
    freopen("C-large.in","r",stdin);
    freopen("C-large.out","w",stdout);
    int test,ans,c[10],l;
    cin>>test;
    for (int tt=1;tt<=test;tt++){
        int s,t;
        ans=0;
        memset(cnt,0,sizeof(cnt));
        cin>>s>>t;
        for (int i=s;i<=t;i++){
            int nt=f(i),now=i;
            //cout<<"came"<<endl;
            l=1;
            c[0]=i;
            while (1){
                  now=now%10*nt+now/10;
                  //cout<<i<<" "<<now<<" "<<nt<<endl;
                  int j;
                  for (j=0;j<l;j++)
                      if (now==c[j]) break;
                  if (now==c[j]) break;
                  if (now>=s && now<=t && now>i)
                     cnt[now]++;
                  c[l++]=now;
                  }
            }
        for (int i=s;i<=t;i++) ans+=cnt[i];    
        cout<<"Case #"<<tt<<": "<<ans<<endl;
        }
}
