#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
const int inf=100000;

class KM_Matching
{
private:
    
    static const int maxn=60;
    
    int n;
    int w[maxn][maxn];
    int lx[maxn];
    int ly[maxn];
    int slack[maxn];
    int D;
    int matchx[maxn];
    int matchy[maxn];
    int piy[maxn];
    
    int ans;
    
    bool DFS_Augment(int u)
    {
        for (int i=0;i<n;i++)
        {
            int v;
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
    
    void Rebuild(int n)
    {
        this->n=n;
        fill(&w[0][0],&w[0][0]+maxn*maxn,-inf);
    }
    
    void Add_Edge(int u,int v,int l)
    {
        w[u][v]=max(w[u][v],l);
    }
    
    int Best_Matching()
    {
        int i,j;
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

#define MAXN 110

vector<int> c0[MAXN];
vector<int> c1[MAXN];
int f[MAXN][MAXN];
KM_Matching KM;

int F(int u,int v)
{
    int n=c0[u].size();
    int m=c1[v].size();
    if (n==0 || m==0) f[u][v]=1;
    else
    {
        int i,j;
        for (i=0;i<n;i++)
        for (j=0;j<m;j++) f[c0[u][i]][c1[v][j]]=F(c0[u][i],c1[v][j]);
        int N=max(n,m);
        KM.Rebuild(N);
        for (i=0;i<N;i++)
        for (j=0;j<N;j++)
        {
            if (i>=n || j>=m) KM.Add_Edge(i,j,0);
            else KM.Add_Edge(i,j,f[c0[u][i]][c1[v][j]]);
        }
        f[u][v]=KM.Best_Matching()+1;
    }
    return f[u][v];
}

int n,m;
set<pair<int,int> > T1;
set<pair<int,int> > T2;
vector<bool> flag;
void Construct_Tree(int u,vector<int> tree[],set<pair<int,int> >& edges,vector<bool>& flag)
{
    flag[u]=false;
    tree[u].clear();
    for (int v=0;v<flag.size();v++)
    if (flag[v] && edges.find(pair<int,int>(u,v))!=edges.end())
    {
        tree[u].push_back(v);
        Construct_Tree(v,tree,edges,flag);
    }
}

int casen;
int main()
{
    freopen("D.in","r",stdin);
    freopen("D.out","w",stdout);
    cin>>casen;
    for (int I=1;I<=casen && printf("Case #%d: ",I);I++)
    {
        int i,j;
        T1.clear();
        T2.clear();
        cin>>n;
        for (i=1;i<n;i++)
        {
            int u,v;
            cin>>u>>v;
            u--;
            v--;
            T1.insert(pair<int,int>(u,v));
            T1.insert(pair<int,int>(v,u));
        }
        Construct_Tree(0,c0,T1,flag=vector<bool>(n,true));
        
        cin>>m;
        for (i=1;i<m;i++)
        {
            int u,v;
            cin>>u>>v;
            u--;
            v--;
            T2.insert(pair<int,int>(u,v));
            T2.insert(pair<int,int>(v,u));
        }
        
        bool ans=false;
        for (i=0;i<m;i++)
        {
            Construct_Tree(i,c1,T2,flag=vector<bool>(m,true));
            for (j=0;j<n;j++)
            if (F(j,i)==m)
            {
                ans=true;break;
            }
            if (ans) break;
        }
        
        if (ans) puts("YES");
        else puts("NO");
    }
    return 0;
}
