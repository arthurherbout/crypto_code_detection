#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const double pi = acos(-1.0);
const double eps = 1e-8;
//const ll INF=(_I64_MAX)/2;
//#pragma comment(linker, "/STACK:102400000,102400000")
const int inf = 0x3f3f3f3f;
#define maxx(a) memset(a, 0x3f, sizeof(a))
#define minn(a) memset(a, 0xC0, sizeof(a))
#define zero(a) memset(a, 0, sizeof(a))
#define FILL(a,b) memset(a, b, sizeof(a))
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)
#define srep(i,n) for(i = 1;i <= n;i ++)
#define MP make_pair
#define fi first
#define se second
typedef pair <int, int> PII;
typedef pair <ll , ll > PX ;

vector<int> st[10];
int n;
string a[10];
bool f[10];
int tot;

int ans;

int seq[10];

void doit() {
    int tpans = 0;
    int i,j,k,t;
    rep(i,n) {
        map<int,int> zz;
        bool ff[11];
        zero(ff);
        rep(j,st[i].size()) {
         //   cout<<st[i][j]<<' ';
            int tp = seq[st[i][j]];
            ff[tp]=1;
            rep(k,a[tp].length()){
                if(a[tp][k]=='1') {
                    zz[k]++;
                }
            }
        }
    //    cout<<"##"<<endl;
        bool err = 0;
        rep(j,n){
            if(ff[seq[j]]==0){
                rep(k,a[seq[j]].length()){
                    if(a[seq[j]][k] == '1' && zz[k] > 0) {
                        err = 1;
                    }
                }
            }
        }
        if (err == 0) {
            int tp_tot = 0;
            int sz = 0;
            for(map<int,int>::iterator it = zz.begin();it != zz.end();it++) {
                if(it->second > 0) {
                    sz ++;
                    tp_tot += it->second;
                }
            }
            if (sz > st[i].size()) return;
            tpans += sz*st[i].size() - tp_tot;
            if (sz < st[i].size()) {
                tpans += (st[i].size() - sz)* st[i].size();
            }
        }
        else{
            return;
        }
    }
   // cout<<tpans<<"@@@"<<endl;
    ans = min(ans, tpans);
}

void jud() {

    int i,j;
    rep(i,n) seq[i] = i;
    doit();
    while(next_permutation(seq,seq+n)){
//        rep(i,n){
//            cout<<seq[i]<<' ';
//        }
//        cout<<endl;
         doit();
    }

}

void dfs(int id,int z) {
    st[z].push_back(id);
    f[z] = 1;
    tot ++;
    if (tot == n) {
        jud();
    }
    int i,j;
    for(i = id + 1;i < n;i ++) {
        dfs(i,z);
        dfs(i,z+1);
    }

    f[z] = 0;
    tot --;
    st[z].pop_back();
}


void work() {
    cin>>n;
    int i,j;
    rep(i,n) {
        cin>>a[i];
      //  cout<<a[i]<<endl;
    }
    zero(f);
    rep(i,n) st[i].clear();
    tot = 0;
    ans = 1000;
    dfs(0,0);
    cout<<ans<<endl;
}

int main() {
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
  //  cout<<a.size()<<' '<<a[2]<<endl;

    int T, cas = 1;
    cin >> T;
    while (T--) {
        printf("Case #%d: ", cas++);
        work();
    }

    return 0;
}



