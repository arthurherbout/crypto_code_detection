/*
 * Bidhan Roy
 * University of Dhaka
 */

using namespace std;

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <ctime>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define rep(i,n) for(__typeof(n) i=0; i<(n); i++)
#define foreach(i,n) for(__typeof((n).begin())i =(n).begin();i!=(n).end();i++)
#define inf (1<<30)
#define eps 1e-9
#define pb push_back
#define ins insert
#define mp make_pair
#define sz(x) ((int)x.size())
#define clr clear()
#define all(x) x.begin(),x.end()
#define xx first
#define yy second
#define sqr(x) ((x)*(x))
#define mem(x,val) memset((x),(val),sizeof(x));
#define read(x) freopen(x,"r",stdin);
#define rite(x) freopen(x,"w",stdout);
#define chk(a,k) ((bool)(a&(1<<(k))))
#define off(a,k) (a&(~(1<<(k))))
#define on(a,k) (a|(1<<(k)))

typedef long long i64;
typedef unsigned long long ui64;
typedef string st;
typedef vector<int> vi;
typedef vector<st> vs;
typedef map<int,int> mii;
typedef map<st,int> msi;
typedef map<int,st> mis;
typedef set<int> si;
typedef set<st> ss;
typedef pair<i64,i64> pii;
typedef vector<pii> vpii;

#define mx 4000010

int d[mx],n[mx],w[mx],e[mx],s[mx];
int dd[mx],dp[mx],ds[mx];

struct data{
    int d,l,r,s;
    bool operator < (const data &b) const {
        return d<b.d;
    }
};

int tree[mx*4];
int up[mx*4];

void upd(int n,int i,int j,int b,int e,int v){
    if(j<b || e<i) return ;
    if(b<=i && j<=e){
        tree[n]=max(v,tree[n]);
        up[n]=max(up[n],v);
        return ;
    }
    int left=n<<1, right=left+1, mid=(i+j)>>1;
    if(up[n]!=-1){
        up[left]=max(up[left],up[n]);
        up[right]=max(up[right],up[n]);
        tree[left]=max(tree[left],up[n]);
        tree[right]=max(tree[right],up[n]);
        up[n]=-1;
    }
    upd(left,i,mid,b,e,v);
    upd(right,mid+1,j,b,e,v);
    tree[n]=min(tree[left],tree[right]);
}

int get(int n,int i,int j,int b,int e){
    if(j<b || e<i) return inf;
    if(b<=i && j<=e){
        return tree[n];
    }
    int left=n<<1, right=left+1, mid=(i+j)>>1;
    if(up[n]!=-1){
        up[left]=max(up[left],up[n]);
        up[right]=max(up[right],up[n]);
        tree[left]=max(tree[left],up[n]);
        tree[right]=max(tree[right],up[n]);
        up[n]=-1;
    }
    return min( get(left,i,mid,b,e),
                get(right,mid+1,j,b,e) );
}

int main(){
    /*int aa,bb,cc;
    while(cin>>aa>>bb>>cc){
        if(aa) cout<<get(1,0,100,bb,cc)<<endl;
        else {
            int v;
            cin>>v;
            upd(1,0,100,bb,cc,v);
        }
    }*/
	double cl = clock();
    cl = clock() - cl;
    read("ccc.in");
    rite("AAAA.txt");
    int test,kas=0;
    cin>>test;
    while(test--){
        printf("Case #%d: ",++kas);
        //mii H;
        int N;
        scanf("%d",&N);
        vector<data> vec;
        rep(i,N) {
            cin>>d[i]>>n[i]>>w[i]>>e[i]>>s[i]>>dd[i]>>dp[i]>>ds[i];
            data temp;
            temp.d=d[i];
            temp.l=w[i];
            temp.r=e[i];
            temp.s=s[i];
            while(n[i]--){
                vec.pb(temp);
                temp.d+=dd[i];
                temp.r+=dp[i];
                temp.l+=dp[i];
                temp.s+=ds[i];
            }
        }
        sort(all(vec));
        mii compress;
        rep(i,sz(vec)){
            vec[i].r--;
            //cout<<vec[i].l<<",,,"<<vec[i].r<<endl;
            compress[ vec[i].l ]=0;
            compress[ vec[i].r ]=0;
        }
        int k=0;
        foreach(it,compress) {

            it->yy=k;
            k+=2;
            //cout<<it->xx<<"-"<<it->yy<<endl;
        }
        rep(i,sz(vec)){
            vec[i].l=compress[ vec[i].l ];
            vec[i].r=compress[ vec[i].r ];
        }
        mem(tree,0);
        mem(up,0);
        int i=0,j=0,ans=0;
        while(i<sz(vec)){
            vector< pair<pii,int> > uuu;
            while(j<sz(vec) && vec[j].d==vec[i].d){
                data temp=vec[j];
                //temp.r--;
                //cout<<temp.d<<" "<<temp.l<<" to "<<temp.r<<" strength="<<temp.s<<endl;
                //vec[j].r--;
                int f=0;
                int mini=get(1,0,k-1,vec[j].l,vec[j].r);
                if(mini<vec[j].s) f=1;
                uuu.pb( mp(pii(vec[j].l,vec[j].r),vec[j].s) );
                //cout<<"mini="<<mini<<endl;
                //cout<<"--->"<<f<<endl;
                ans+=f;
                j++;
            }
            rep(kk,sz(uuu)) upd(1,0,k-1,uuu[kk].xx.xx,uuu[kk].xx.yy,uuu[kk].yy);
            i=j;
        }
        cout<<ans<<endl;
    }
    fprintf(stderr, "Total Execution Time = %lf seconds", cl / CLOCKS_PER_SEC);
    return 0;
}
