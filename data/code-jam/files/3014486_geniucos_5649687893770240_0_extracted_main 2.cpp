#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
int mod,m,T,pt,cul[109],msk,maxi,TT,i,j,n,dp[1<<15],l[109];
long long cnt;
char s[109][109];
struct nod
{
    nod *urm[26];
    nod()
    {
        memset(urm,0,sizeof(urm));
    }
}*q;
int tot(nod *&R)
{
    int i,t=1;
    for(i=0;i<26;i++)
        if(R->urm[i]!=0) t+=tot(R->urm[i]);
    return t;
}
int Det(int msk)
{
    int i,j;
    nod *R = new nod;
    for(i=0;i<n;i++)
        if(msk&(1<<i))
        {
            q=R;
            for(j=1;j<=l[i];j++)
            {
                if(q->urm[s[i][j]-'A']==0) q->urm[s[i][j]-'A']=new nod;
                q=q->urm[s[i][j]-'A'];
            }
        }
    return tot(R);
}
void back(int poz)
{
    if(poz==n)
    {
        TT=0;
        for(i=1;i<=m;i++)
        {
            msk=0;
            for(j=0;j<n;j++)
                if(cul[j]==i) msk|=1<<j;
            if(msk==0) break;
            TT+=dp[msk];
        }
        if(i<=m) return;
        if(TT>maxi) maxi=TT,cnt=1;
        else
        if(TT==maxi) cnt++;
        return ;
    }
    int j;
    for(j=1;j<=m;j++)
    {
        cul[poz]=j;
        back(poz+1);
    }
}
int main()
{
freopen("input","r",stdin);
freopen("output","w",stdout);
scanf("%d",&T);
while(T)
{
    pt++;
    T--;
    printf("Case #%d: ",pt);
    scanf("%d%d\n",&n,&m);
    mod=1000000007;
    for(i=0;i<n;i++)
    {
        gets(s[i]+1);
        l[i]=strlen(s[i]+1);
    }
    for(i=0;i<(1<<n);i++)
        dp[i]=Det(i);
    maxi=0;
    cnt=0;
    back(0);
    printf("%d %lld\n",maxi,cnt%mod);
}
return 0;
}
