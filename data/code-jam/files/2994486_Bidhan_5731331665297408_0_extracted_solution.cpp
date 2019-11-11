/*
 * Bidhan Roy
 * University of Dhaka
 */

using namespace std;
#include <bits/stdc++.h>

#define foreach(i,n) for(__typeof((n).begin())i =(n).begin();i!=(n).end();i++)
#define sgn(x,y) ((x)+eps<(y)?-1:((x)>eps+(y)?1:0))
#define rep(i,n) for(__typeof(n) i=0; i<(n); i++)
#define mem(x,val) memset((x),(val),sizeof(x));
#define rite(x) freopen(x,"w",stdout);
#define read(x) freopen(x,"r",stdin);
#define all(x) x.begin(),x.end()
#define sz(x) ((int)x.size())
#define sqr(x) ((x)*(x))
#define pb push_back
#define mp make_pair
#define clr clear()
#define inf (1<<30)
#define ins insert
#define xx first
#define yy second
#define eps 1e-9

typedef long long i64;
typedef unsigned long long ui64;
typedef string st;
typedef vector<int> vi;
typedef vector<st> vs;
typedef map<int,int> mii;
typedef map<st,int> msi;
typedef set<int> si;
typedef set<st> ss;
typedef pair<int,int> pii;
typedef vector<pii> vpii;

#define mx 20

st num[mx];
vi edge[mx];

int color[mx];

vi city;

void call(int u,int &idx,int prev=-1){
    //cout<<"Entered u="<<u<<" from "<<prev<<endl;
    idx++;
    if(idx==sz(city)) return ;
    while(true){
        bool br=false;
        if(idx==sz(city)) return ;
        foreach(it,edge[u]) {
            //cout<<" in "<<u<<" "<<*it<<endl;
            if(*it==prev) continue;
            if(*it==u) continue;
            if(*it==city[idx]){
                call(*it,idx,u);
                br=true;
            }
        }
        if(!br) break;
    }
}

int main(){
    time_t start=clock();
    read("in.txt");
    rite("out.txt");

    ios_base::sync_with_stdio(0);

    int TEST, KAS=0;
    cin>>TEST;
    while( TEST-- ){
        int n,m;
        cin>>n>>m;
        rep(i,n) cin>>num[i], edge[i].clr;
        while( m-- ){
            int x,y;
            cin>>x>>y;
            x--, y--;
            edge[x].pb(y);
            edge[y].pb(x);
        }
        city.clear();

        rep(i,n) city.pb(i);
        st ans="999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
        do{
            /*if(city[0]!=2) continue;
            if(city[1]!=1) continue;
            if(city[2]!=0) continue;
            cout<<"running "<<endl;
            rep(i,sz(city)) cout<<" "<<city[i]; cout<<endl;*/
            int idx=0;
            call(city[0],idx);
            //cout<<"idx="<<idx<<endl;
            if(idx!=sz(city)) continue;
            st temp="";
            rep(i,sz(city)) temp+=num[ city[i] ];
            ans=min(ans,temp);
        }while( next_permutation( all(city) ) );

        cout<<"Case #"<<++KAS<<": ";
        cout<<ans<<endl;
        //return 0;
    }

    cerr << " Program has run "<< ( clock()-start ) / 1000 << " s " << endl;
    return 0;
}
