#include <iostream>
#include <algorithm>
using namespace std;

const long long inf=((long long)1000000)*((long long)1000000);
const long long maxn=1000;

class KM_Matching
{
private:
    
    long long n;
    long long w[maxn][maxn];
    long long lx[maxn];
    long long ly[maxn];
    long long slack[maxn];
    long long D;
    long long matchx[maxn];
    long long matchy[maxn];
    long long piy[maxn];
    
    long long ans;
    
    bool DFS_Augment(long long u)
    {
        for (long long i=0;i<n;i++)
        {
            long long v;
            if (lx[u]+ly[i]==w[u][i])
            {
                if (piy[i]==-1 && i!=matchx[u])
                {
                    piy[i]=u;
                    v=matchy[i];
                    if (v==-1)
                    {
                        matchy[i]=u;
                        matchx[u]=i;
                        return true;
                    }
                    else
                    {
                        if (DFS_Augment(v))
                        {
                            matchy[i]=u;
                            matchx[u]=i;
                            return true;
                        }
                    }
                }
            }
            else slack[i]=min(slack[i],lx[u]+ly[i]-w[u][i]);
        }
        return false;
    }
    
public:
    
    void Rebuild(long long n)
    {
        this->n=n;
        fill(&w[0][0],&w[0][0]+maxn*maxn,-inf);
    }
    
    void Add_Edge(long long u,long long v,long long l)
    {
        w[u][v]=max(w[u][v],l);
    }
    
    long long Best_Matching()
    {
        long long i,j;
        for (i=0;i<n;i++)
        {
            lx[i]=-inf;
            for (j=0;j<n;j++) lx[i]=max(lx[i],w[i][j]);
            ly[i]=0;
        }
        memset(matchx,-1,sizeof(matchx));
        memset(matchy,-1,sizeof(matchy));
        for (i=0;i<n;i++)
        {
            memset(piy,-1,sizeof(piy));
            fill(slack,slack+n,inf);
            while (!DFS_Augment(i))
            {
                D=inf;
                for (j=0;j<n;j++) D=min(D,slack[j]);
                for (j=0;j<n;j++)
                {
                    if (piy[j]!=-1)
                    {
                        ly[j]+=D;
                        lx[matchy[j]]-=D;
                    }
                }
                lx[i]-=D;
                memset(piy,-1,sizeof(piy));
                fill(slack,slack+n,inf);
            }
        }
        ans=0;
        for (i=0;i<n;i++) ans+=lx[i]+ly[i];
        return ans;
    }
    
};

KM_Matching M;
long long n;
long long x[maxn],y[maxn];

long long t;
int main()
{
freopen("A.in","r",stdin);
freopen("A.out","w",stdout);
    cin>>t;
    for (long long i=1;i<=t && cout<<"Case #"<<i<<": ";i++)
    {
        long long i,j,k;
        cin>>n;
        for (i=0;i<n;i++) cin>>x[i];
        for (j=0;j<n;j++) cin>>y[j];
        M.Rebuild(n);
        for (i=0;i<n;i++)
        for (j=0;j<n;j++) M.Add_Edge(i,j,inf-x[i]*y[j]);
        cout<<inf*n-M.Best_Matching()<<endl;
    }
    return 0;
}
