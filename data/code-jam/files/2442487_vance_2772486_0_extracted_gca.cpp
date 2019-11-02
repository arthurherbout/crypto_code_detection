#include<cstdio>
#include<cmath>
#include<algorithm>
#include<vector>
#include<map>
using namespace std;
typedef long long LL;
typedef pair<int,int> PII;
#define PB push_back
#define MP make_pair
map<int,int> stp, edp;
int main()
{
    int t;
    LL n,m;
    int i,j;
    LL mod = 1000002013ll;
    freopen("a.in","r",stdin);
    freopen("a.out","w",stdout);
    scanf("%d",&t);
    for(int cnt = 1;cnt<=t;cnt++)
    {
        scanf("%lld%lld",&n,&m);
        stp.clear();
        edp.clear();
        vector<PII>v;
        LL sum = 0;
        while(m--)
        {
            int s,e;
            int p;
            scanf("%d%d%d",&s,&e,&p);
            LL tt = 1ll*(2*n+1-e+s)*(e-s)/2;
            sum += (tt%mod*p)%mod;
            sum %= mod;
            v.PB(MP(s,e));
            stp[s]+=p;
            edp[e]+=p;
        }
        sort(v.begin(),v.end());
        //for(i=0;i<v.size();i++)
            //printf("%d %d %d %d\n",v[i].first,v[i].second,stp[v[i].first],edp[v[i].second]);
        LL ns = 0;
        //i = j = 0;
        i = v.size()-1;
        int f=1;
        while(i>=0)
        {
            int start = v[i].first;
            int end = n;
            for(j=0;j<v.size();j++)
                if(v[j].second>=start&&edp[v[j].second]>0)
                    end = min(end,v[j].second);
            /*for(j=i+1;j<v.size();j++)
            {
                if(v[j].first<=end)
                {

                    //printf("%d\n",edp[v[j].second]);
                    if(edp[v[j].second]>0&&v[j].second>end)
                        end = v[j].second;
                }
            }*/
            //if(start!=3)printf("%d->%d\n",start,end);
            int pas = min(stp[start],edp[end]);
            LL tt = 1ll*(2*n+1-end+start)*(end-start)/2;
            ns += (tt%mod*pas)%mod;
            ns %= mod;
            stp[start] -= pas;
            edp[end] -= pas;
            while(i>=0&&!stp[v[i].first])
                i--;
            /*if(i!=44)printf("%d %d %d %d %d\n",i,start,end,stp[start],edp[end]);
            if(i==44&&f==1)
            {
                f=0;
                for(j=0;j<70;j++)
                    if(stp[j])
                        printf("%d,%d ",j,stp[j]);
                printf("\n");
                for(j=0;i<70;j++)
                    if(edp[j])
                        printf("%d,%d ",j,edp[j]);
                printf("\n");
            }*/
        }
        //printf("%lld %lld\n",sum,ns);
        LL ret = (sum - ns + mod)%mod;
        printf("Case #%d: %lld\n",cnt,ret);
    }
    return 0;
}
