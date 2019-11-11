#include<cstdio>
#include<algorithm>
using namespace std;
int nr,T,pt,n,m,i,j,k,KK;
long long mini,INF,ras[109],H[109],dp[10][1<<9],muc[10][10],X[109],bit[109],msk[109],unde[109],realval[109];
struct str
{
    int p,v;
}a[100];
bool cmp(str a,str b){return a.v<b.v;}
long long concate(long long p,long long b)
{
    int lg,cif[109];
    lg=0;
    while(b) cif[++lg]=b%10,b/=10;
    reverse(cif+1,cif+lg+1);
    for(int pp=1;pp<=lg;pp++)
        p=1LL*p*10+cif[pp];
    return p;
}
long long MI(int rad,int msk)
{
    int j;
    long long R=INF;
    for(j=0;j<n;j++)
        if((msk&(1<<j))&&muc[j][rad]&&dp[j][msk]<R)
            R=dp[j][msk];
    return R;
}
bool Cmp(long long a,long long b)
{
    return concate(a,b)<concate(b,a);
}
void back(int poz,int vu)
{
    if(poz==nr+1)
    {
        int nr0=0;
        long long H[109];
        for(KK=1;KK<=vu;KK++)
        {
            H[++nr0]=MI(j,msk[KK]);
            if(H[nr0]>=INF) break;
        }
        if(KK<=vu) return ;
        sort(H+1,H+nr0+1,Cmp);
        long long p=0;
        for(KK=1;KK<=nr0;KK++)
        {
            /////ii pun lui p la sfarsit pe H[KK]
            p=concate(p,H[KK]);
        }
        if(p<dp[j][i]) dp[j][i]=p;
        return ;
    }
    int i;
    for(i=1;i<=vu+1;i++)
    {
        unde[poz]=i;
        msk[i]|=1<<bit[poz];
        if(i<=vu) back(poz+1,vu);
        else back(poz+1,vu+1);
        msk[i]-=1<<bit[poz];
    }
}
int main()
{
//freopen("input","r",stdin);
//freopen("output","w",stdout);
INF=1000000000;
scanf("%d",&T);
while(T)
{
    pt++;
    T--;
    printf("Case #%d: ",pt);
    scanf("%d%d",&n,&m);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&a[i].v);
        a[i].p=i;
    }
    while(m)
    {
        m--;
        scanf("%d%d",&i,&j);
        i--;j--;
        muc[i][j]=muc[j][i]=1;
    }
    sort(a+1,a+n+1,cmp);
    X[a[1].p]=1;
    realval[1]=a[1].v;
    for(i=2;i<=n;i++)
    {
        X[a[i].p]=X[a[i-1].p];
        if(a[i].v!=a[i-1].v)
        {
            X[a[i].p]++;
            realval[X[a[i].p]]=a[i].v;
        }
    }
    for(i=1;i<(1<<n);i++)
        for(j=0;j<n;j++)
            if(i&(1<<j))
            {
                if(j==4&&i==(1<<n)-1)
                    j=4;
                nr=0;
                for(k=0;k<n;k++)
                    if((i&(1<<k))&&k!=j) bit[++nr]=k;
                dp[j][i]=INF;
                back(1,0);
                if(dp[j][i]!=INF) dp[j][i]=concate(X[j+1],dp[j][i]);
            }
    mini=INF;
    for(j=0;j<n;j++)
        if(dp[j][(1<<n)-1]<mini) mini=dp[j][(1<<n)-1];
    nr=0;
    while(mini)
    {
        ras[++nr]=realval[mini%10];
        mini/=10;
    }
    for(i=nr;i>=1;i--)
        printf("%d",ras[i]);
    printf("\n");
    for(i=0;i<=n;i++)
        for(j=0;j<=n;j++)
            muc[i][j]=0;
}
return 0;
}
