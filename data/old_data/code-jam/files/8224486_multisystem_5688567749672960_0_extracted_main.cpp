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

int rev(int num)
{
     sstr ss;
     string str;
     ss << num;
     ss >> str;
     reverse(all(str));
     sstr ss2(str);
     ss2 >> num;
     return num;
}
vi res;
void BFS()
{
     res = vi(1000006,1<<27);
     queue<int> q;
     q.push(1);
     res[1]=1;
     int step = 1;
     while (q.size()) {
          int siz = q.size();
          while (siz--) {
               int cur = q.front();

               q.pop();
               if (cur+1 < 1000006 && res[cur+1] > step+1) {
                    res[cur+1] = step+1;
                    q.push(cur+1);
               }
               int r =rev(cur);
               if (r < 1000006 && res[r] > step+1) {
                    res[r] = step+1;
                    q.push(r);
               }
          }
          step++;
     }

}
int main()
{
     ios_base::sync_with_stdio(0);
     cin.tie(0);
#ifndef ONLINE_JUDGE
     freopen("main.txt", "rt", stdin);
     freopen("out.txt", "wt", stdout);
#endif

     BFS();
     int tst,n;
     cin >> tst;
     repa(tt,1,tst,1) {
          cin >> n;
          printf("Case #%d: %d\n",tt,res[n]);
     }


}