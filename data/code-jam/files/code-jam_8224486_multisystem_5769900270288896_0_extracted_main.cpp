#include <bits/stdc++.h>

#define rep(i,n) for(int i=0; i<n; i++)
#define repa(i,a,b,d) for(int i=a; i<=b; i+=d)
#define repd(i,a,b,d) for(int i=a; i>=b; i-=d)
#define repi(it,stl) for(auto it = (stl).begin(); (it)!=stl.end(); ++(it))
#define sz(a) ((int)(a).size())
#define mem(a,n) memset((a), (n), sizeof(a))
#define all(n) (n).begin(),(n).end()
#define rall(n) (n).rbegin(),(n).rend()
#define mt make_tuple
#define tii tuple<int,int>
#define at(t,idx) std::get<(idx)>((t))
#define vi vector<int>
#define vs vector<string>
#define sstr stringstream
#define indexof(v,x) (int((find((v).begin(), (v).end(),(x))-(v).begin() + 1)%((v).size()+1))-1)

typedef long long ll;
using namespace std;


int r,c;
unordered_map<int, int > memo[16][16][17][17];
int DP(int idx, int rem, bitset<16> msk)
{
     if (rem == -1) return 0;
     if (idx == r*c) return 1<<27;

     if (memo[idx][rem][r][c].count(msk.to_ulong())) return memo[idx][rem][r][c][msk.to_ulong()];
     int res = DP(idx+1, rem, msk);

     int up = idx-c >= 0?msk[idx-c]:0;
     int left = idx%c==0?0:msk[idx-1];
     bitset<16> cp = msk;
     cp[idx]=1;
     res = min(res, DP(idx+1, rem-1, cp)+up+left);

     return memo[idx][rem][r][c][msk.to_ulong()] = res;

}
int main()
{
     ios_base::sync_with_stdio(0);
     cin.tie(0);
#ifndef ONLINE_JUDGE
     freopen("main.txt", "rt", stdin);
     freopen("out.txt", "wt", stdout);
#endif

     int tst;
     cin >> tst;
     repa(tt,1,tst,1) {
          int n;
          cin >> r >> c >> n;
          int res = DP(0,n-1,0);
          printf("Case #%d: %d\n",tt,res);
     }
     return 0;
}