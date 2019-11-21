/* 
 * File:   Problem_A.cpp
 * Author: Kimi
 *
 * Created on 2009年9月13日, 上午12:07
 */

#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <complex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#define Fill(A,n) memset(A,n,sizeof(A))
#define pb push_back

using namespace std;

/*
 *
 */
struct Tnode
{
    double ratio;
    int type,yes,no;
};
vector<Tnode> nd;
map<int,int> feature;
map<string,int> g;
int mn;
stringstream st;

void dfs()
{
    string s;
    Tnode tn;
    st >> s;
    st >> tn.ratio;
    st >> s;
    if (s==")") tn.type=0;
    else {
         if (!g[s]) g[s]=++mn;
         tn.type=g[s];
         dfs();
         tn.yes=nd.size()-1;
         dfs();
         tn.no=nd.size()-1;
         st >> s;
    }
    nd.pb(tn);
}

double work(int pst, double ratio)
{
    ratio*=nd[pst].ratio;
    if (!nd[pst].type) return ratio;
    if (feature[nd[pst].type]) return work(nd[pst].yes,ratio);
    else return work(nd[pst].no,ratio);
}

int main() {
    int tt;
    scanf("%d",&tt);
    for (int t=0; t<tt; t++) {
        printf("Case #%d:\n",t+1);
        int L;
        string s;
        st.clear();
        scanf("%d",&L);
        getline(cin,s);
        for (int i=0; i<L; i++) {
            getline(cin,s);
            for (int j=0; j<s.size(); j++)
                if (s[j]=='(' || s[j]==')') st<< " " << s[j] << " ";
                else st << s[j];
            st << " ";
        }
        mn=0;
        g.clear();
        nd.clear();
        dfs();
//        for (int i=0; i<nd.size(); i++) {
//            if (nd[i].type) printf("ratio:%.8lf left:%d right:%d feature:%d\n",nd[i].ratio,nd[i].yes,nd[i].no,nd[i].type);
//            else printf("ratio:%.8lf\n",nd[i].ratio);
//        }
//        for (map<string,int>::iterator iter=g.begin(); iter!=g.end(); iter++)
//            cout << iter->first << ":" << iter->second << endl;
        int n;
        scanf("%d",&n);
        for (int tp=0; tp<n; tp++) {
            feature.clear();
            string name;
            cin >> name;
            int m;
            scanf("%d",&m);
            for (int i=0; i<m; i++) {
                string ss;
                cin >> ss;
                feature[g[ss]]=1;
            }
            printf("%.8lf\n",work(nd.size()-1,1));
        }
    }
    return (EXIT_SUCCESS);
}

