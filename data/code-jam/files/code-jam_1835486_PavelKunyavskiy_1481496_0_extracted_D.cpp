#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <queue>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <stack>
#define mp make_pair
#define pb push_back   
#define setval(a,v) memset(a,v,sizeof(a))
using namespace std;

typedef long long ll;
typedef long double ld;

const string TASKNAME = "D";
const string TASKMOD = "small";
const string INFILE = TASKNAME+"-"+TASKMOD+".in";
const string OUTFILE = TASKNAME+"-"+TASKMOD+".out";


//"o" to "0", "i" to "1", "e" to "3", "a" to "4", "s" to "5", "t" to "7", "b" to "8" and/or "g" to "9".

const string eqv = "oieastbg";
const string eqvto = "01345789";

int ans;
vector<int> g[36];
vector<int> gt[36];
bool used[36];

void PreCalc(){
}

void clear(){
    for (int i = 0; i < 36; i++)
        g[i].clear(),gt[i].clear();
}


void addone(int a,int b){
    g[a].pb(b);
    gt[b].pb(a);
}
void add(char c1,char c2){
    addone(c1-'a',c2-'a');
    char _c1 = 0, _c2 = 0;
    for (int i = 0; i < (int)eqv.size(); i++){
        if (c1 == eqv[i]) _c1 = eqvto[i];
        if (c2 == eqv[i]) _c2 = eqvto[i];
    }


    if (_c1) addone(_c1-'0'+26,c2-'a');
    if (_c2) addone(c1-'a',_c2-'0'+26);
    if (_c1 && _c2) addone(_c1-'0'+26,_c2-'0'+26);

}

void solve(){
    int k;
    string s;
    cin >> k >> s;
    if (k != 2){
        cout << "It's to hard"<< endl;
        cerr << "WARN!"<<endl;
        return ;
    }
    clear();
    
    for (int i = 1; i < (int)s.size(); i++)
        add(s[i-1],s[i]);

    memset(used,0,sizeof(used));

    bool any = 0;
    ans = 0;

    for (int i = 0; i < 36; i++){
        sort(g[i].begin(),g[i].end());
        g[i].erase(unique(g[i].begin(),g[i].end()),g[i].end());

        sort(gt[i].begin(),gt[i].end());
        gt[i].erase(unique(gt[i].begin(),gt[i].end()),gt[i].end());


        ans += abs((int)g[i].size() - (int)gt[i].size());
        ans += g[i].size() + gt[i].size();
        if (g[i].size() != gt[i].size())
            any = true;

        //if (g[i].size() || gt[i].size())
        //    cerr << i <<" "<<g[i].size() <<" "<<gt[i].size() << endl;
    }
    ans /= 2;
    ans+= !any;
    cout << ans << endl;
}

int main(){
    freopen(INFILE.data(),"r",stdin);
    freopen(OUTFILE.data(),"w",stdout);
    PreCalc();
    int t;
    cin>>t;
    for (int i=1;i<=t;i++)
        {
            printf("Case #%d: ",i);
            solve();
            if (i>t-20 || i%10==0)
                cerr<<"SOLVED:"<<i<<endl;
        }

  return 0;
}