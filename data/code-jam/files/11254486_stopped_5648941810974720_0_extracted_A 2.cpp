#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for(i=0;i<n;i++)
#define rep1(i,n) for(i=1;i<=n;i++)
#define rev(i,n) for(i=n-1;i>=0;i--)
#define rev1(i,n) for(i=n;i>=1;i--)
#define REP(i,a,b) for(i=a;i<b;i++)
#define REP1(i,a,b) for(i=a;i<=b;i++)
#define FOR(it,v) for(auto it=v.begin();it!=v.end();it++)
#define pb push_back
#define pq priority_queue
#define mem(i,n) memset(i,n,sizeof(i))
#define eps 1e-7
#define debug printf("bug!!!\n")
#define sz size()
#define em empty()
#define cl clear()
#define fi first
#define se second
#define mp make_pair
#define Sort(v) sort( v.begin(),v.end() )

/*Knight direction*/
long kx[]={2,-2,2,-2,1,-1,1,-1};
long ky[]={1,1,-1,-1,2,2,-2,-2};
/*4 & 8 move direction*/
long dx[]={1,-1,0,0,1,-1,1,-1};
long dy[]={0,0,1,-1,1,1,-1,-1};
//#define check(a,b,row,column) ((a>=0 && a<row) && (b>=0 && b<column))

/*Bit Mask DP Macros*/
#define Set(n,pos) ( n | (1<<pos) )
#define reset(n,pos) (n & (~(1<<pos)))
#define check(n,pos) ((bool)(n&(1<<pos)))

/*input && output*/
#define sf3(x,y,z) scanf("%ld %ld %ld",&x,&y,&z)
#define sf2(x,y) scanf("%ld %ld",&x,&y)
#define sf1(x) scanf("%ld",&x)
#define pf3(x,y,z) printf("%ld %ld %ld\n",x,y,z)
#define pf2(x,y) printf("%ld %ld\n",x,y)
#define pf1(x) printf("%ld\n",x)
#define NL puts("")
/*typedef*/
typedef long L;
typedef long long LL;
const L INF= (1e9);
inline L MOD(L a,L b)
{
    L res=a%b;
    return res<0 ? res+abs(b) : res;
}
inline L Pow(L base, L exp)
{
    L res = 1;
    while (exp>0)
    {
        if (exp & 1) res*= base;
        exp >>= 1;
        base *= base;
    }
    return res;
}
inline L Log(L base,L n)
{
    L ans=(L) ( floor( log10( (double)n ) / log10( (double)base ) )+eps );
    return ans;
}
inline L BigMod(L base, L exp,L M)
{
    base=MOD(base,M);
    L res = 1;
    while (exp>0)
    {
        if (exp & 1) res= MOD(res*base,M);
        exp >>= 1;
        base=MOD(base*base,M);
    }
    return res;
}
typedef pair<L,L> pll;
typedef vector <L> vl;
typedef map < string, L > msl;
typedef map < L,string > mls;
typedef map < char, L > mcl;
typedef map < L,char > mlc;
typedef map < L, L > mll;
typedef queue <L> Queue;
typedef vector <string> vs;
typedef vector <pll> vp;
#define mx 109
L ar[mx];
L aux[10],res[10];
int main()
{
    //freopen("A-small-attempt3.in","r",stdin);
    //freopen("A-small-attempt5.in","r",stdin);
    //freopen("output.txt","w",stdout);
    L x,y,z,i,j,k,l,m,n,p,q,r,u,v,w,t,cas,edge,node,a,b,c,d,e,f,ans,cnt,ass;
    sf1(t);
    rep1(cas,t)
    {
        string s;
        cin>>s;
        mem(ar,0);
        mem(res,0);
        rep(i,s.sz)
        {
            ar[s[i]]++;
        }

        if(ar['Z'])
        {
            aux[0]=ar['Z'];
            aux[1]=ar['E'];
            aux[2]=ar['R'];
            aux[3]=ar['O'];
            sort(aux,aux+4);
            ar['Z']-=aux[0];
            ar['E']-=aux[0];
            ar['R']-=aux[0];
            ar['O']-=aux[0];
            res[0]=aux[0];
        }

        if(ar['W'])
        {
            aux[0]=ar['T'];
            aux[1]=ar['W'];
            aux[2]=ar['O'];
            sort(aux,aux+3);
            ar['T']-=aux[0];
            ar['W']-=aux[0];
            ar['O']-=aux[0];
            res[2]=aux[0];
        }


        if(ar['U'])
        {
            aux[0]=ar['F'];
            aux[1]=ar['O'];
            aux[2]=ar['U'];
            aux[3]=ar['R'];
            sort(aux,aux+4);
            ar['F']-=aux[0];
            ar['O']-=aux[0];
            ar['U']-=aux[0];
            ar['R']-=aux[0];
            res[4]=aux[0];
        }

        if(ar['X'])
        {
            aux[0]=ar['S'];
            aux[1]=ar['I'];
            aux[2]=ar['X'];
            sort(aux,aux+3);
            ar['S']-=aux[0];
            ar['I']-=aux[0];
            ar['X']-=aux[0];
            res[6]=aux[0];
        }


        if(ar['G'])
        {
            aux[0]=ar['E'];
            aux[1]=ar['I'];
            aux[2]=ar['G'];
            aux[3]=ar['H'];
            aux[4]=ar['T'];
            sort(aux,aux+5);
            ar['E']-=aux[0];
            ar['I']-=aux[0];
            ar['G']-=aux[0];
            ar['H']-=aux[0];
            ar['T']-=aux[0];
            res[8]=aux[0];
        }

        if(ar['O'])
        {
            aux[0]=ar['O'];
            aux[1]=ar['N'];
            aux[2]=ar['E'];
            sort(aux,aux+3);
            ar['O']-=aux[0];
            ar['N']-=aux[0];
            ar['E']-=aux[0];
            res[1]=aux[0];
        }

        if(ar['R'])
        {
            aux[0]=ar['T'];
            aux[1]=ar['H'];
            aux[2]=ar['R'];
            aux[3]=ar['E']/2;
            //aux[4]=ar['E'];
            sort(aux,aux+4);
            ar['T']-=aux[0];
            ar['H']-=aux[0];
            ar['R']-=aux[0];
            ar['E']-=(aux[0]*2);
            //ar['E']-=aux[0];
            res[3]=aux[0];
        }



        if(ar['F'])
        {
            aux[0]=ar['F'];
            aux[1]=ar['I'];
            aux[2]=ar['V'];
            aux[3]=ar['E'];
            sort(aux,aux+4);
            ar['F']-=aux[0];
            ar['I']-=aux[0];
            ar['V']-=aux[0];
            ar['E']-=aux[0];
            res[5]=aux[0];
        }

        if(ar['V'])
        {
            aux[0]=ar['S'];
            aux[1]=ar['E']/2;
            aux[2]=ar['V'];
            //aux[3]=ar['E'];
            aux[3]=ar['N'];
            sort(aux,aux+4);
            ar['S']-=aux[0];
            ar['E']-=(aux[0]*2);
            ar['V']-=aux[0];
            //ar['E']-=aux[0];
            ar['N']-=aux[0];
            res[7]=aux[0];
        }

        if(ar['I'])
        {
            aux[0]=ar['N'];
            aux[1]=ar['I'];
            //aux[2]=ar['N'];
            aux[2]=ar['E'];
            sort(aux,aux+3);
            ar['N']-=(aux[0]*2);
            ar['I']-=aux[0];
            //ar['N']-=aux[0];
            ar['E']-=aux[0];
            res[9]=aux[0];
        }


        printf("Case #%ld: ",cas);
        rep(i,10)
        {
            rep(j,res[i]) cout<<i;
        }
        NL;
        //printf("Case #%ld: %ld\n",cas,ans);
    }

    return 0;
}


