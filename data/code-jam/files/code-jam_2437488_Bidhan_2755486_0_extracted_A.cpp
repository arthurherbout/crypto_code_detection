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

#define mx 200000

int d[mx],n[mx],w[mx],e[mx],s[mx];
int dd[mx],dp[mx],ds[mx];

struct data{
    int d,l,r,s;
    bool operator < (const data &b) const {
        return d<b.d;
    }
};

int main(){
	double cl = clock();
    cl = clock() - cl;
    read("Ain.in");
    rite("cout.txt");
    int test,kas=0;
    cin>>test;
    while(test--){
        printf("Case #%d: ",++kas);
        mii H;
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
        int i=0,j=0,ans=0;
        while(i<sz(vec)){
            mii TT;
            while(j<sz(vec) && vec[j].d==vec[i].d){
                //data temp=vec[j];
                //temp.r--;
                //cout<<temp.d<<" "<<temp.l<<" to "<<temp.r<<" strength="<<temp.s<<endl;
                vec[j].r--;
                int f=0;
                for(int k=vec[j].l; k<=vec[j].r; k++){
                    if(vec[j].s>H[k]){
                        //cout<<k<<" attacked "<<endl;
                        TT[k]=max(TT[k],vec[j].s);
                        f=1;
                    }
                }
                //cout<<"--->"<<f<<endl;
                ans+=f;
                j++;
            }
            foreach(it,TT) H[it->xx]=max(it->yy,H[it->xx]);
            i=j;
        }
        cout<<ans<<endl;
    }
    fprintf(stderr, "Total Execution Time = %lf seconds", cl / CLOCKS_PER_SEC);
    return 0;
}
