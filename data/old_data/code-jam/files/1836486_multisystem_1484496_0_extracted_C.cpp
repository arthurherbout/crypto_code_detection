#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>

#define rep(i,n) for(int i=0; i<n; i++)
#define reps(i,m,n) for(int i=m; i<n; i++)
#define repe(i,m,n) for(int i=m; i<=n; i++)
#define repi(it,stl) for(typeof((stl).begin()) it = (stl).begin(); (it)!=stl.end(); ++(it))
#define sz(a) ((int)(a).size())
#define mem(a,n) memset((a), (n), sizeof(a))
#define all(n) (n).begin(),(n).end()
#define rall(n) (n).rbegin(),(n).rend()
#define mp(a,b) make_pair((a),(b))
#define pii pair<int,int>
#define vi vector<int>
#define vs vector<string>
#define sstr stringstream 
#define fnd(v,x) (find(all((v)),(x))!=(v).end())
#define itr(A,B) typeof(B) A = B
typedef long long ll;
using namespace std;

int n;
ll arr[502];
ll seq1[502],seq2[502];
ll cum[502];
int idx1,idx2;
bool rec(int idx, ll sum, bool taken){
    if(sum == 0 && taken) return 1;
    if(idx == n) return 0;
    ll rem = cum[n-1]-cum[idx]+arr[idx];
    if(sum < 0 && sum+rem < 0) return 0;
    if(sum > 0 && sum-rem > 0) return 0;


    seq1[idx1++]=arr[idx];
    if(rec(idx+1,sum+arr[idx],1))
        return 1;
    --idx1;
    if(rec(idx+1,sum,taken))
        return 1;

    if(taken){
        seq2[idx2++] = arr[idx];
        if(rec(idx+1,sum-arr[idx],1))
            return 1;
        --idx2;
    }

    return 0;
}

int main(){
#ifndef ONLINE_JUDGE
    freopen("in","rt",stdin);
    freopen("out","wt",stdout);
#endif

    int tst;
    scanf("%d",&tst);
    repe(t,1,tst){
        idx1=idx2=0;
        scanf("%d",&n);
        rep(i,n){
            scanf("%lld",arr+i);
            cum[i]=i==0?arr[i]:arr[i]+cum[i-1];
        }
        printf("Case #%d:\n",t);
        if(!rec(0,0,0))
            printf("Impossible\n");
        else{
            char* en="";
            rep(i,idx1)
                    printf("%s%lld",en,seq1[i]),en=" ";
            en = "\n";
            rep(i,idx2)
                    printf("%s%lld",en,seq2[i]),en=" ";
            printf("\n");
        }
    }
    
}





