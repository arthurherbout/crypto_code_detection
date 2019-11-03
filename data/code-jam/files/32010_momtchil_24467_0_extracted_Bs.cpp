#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <numeric>
#include <functional>
#include <queue>
#include <bitset>

using namespace std;

#define PI pair<int,int>
#define MP make_pair
#define VI vector<int>
#define LL long long
#define ULL unsigned LL
#define PB push_back
#define x first
#define y second
#define SZ size()
#define ALL(v) (v).begin(),(v).end()
#define pause system("pause")
#define Repi(n) for (int i=0;i<n;i++)
#define Repj(n) for (int j=0;j<n;j++)
#define Repk(n) for (int k=0;k<n;k++)
#define sign(x) (((x)>0)-((x)<0))

int T,N,M;

int main()
{
    scanf("%d",&T);
    string s[15]; int a[15],b[15]; set<string> used;
    Repi(T)
     {
           scanf("%d",&N);
           int ans=1000000000;
           Repj(N)
            {
                  cin>>s[j]>>a[j]>>b[j];
            }
         //  cout<<"--------------case "<<i<<":\n";
           vector<PI> is;
           for (int mask=1;mask<(1<<N);mask++)
            {
                    if (__builtin_popcount(mask)>=ans) continue;
                    used.clear(); is.clear();
                    Repj(N)
                     if (mask&(1<<j))
                      {
                                     used.insert(s[j]); if (used.SZ>3) break;
                                     is.PB(MP(a[j],-1)); is.PB(MP(b[j]+1,1));
                      }
                //   cout<<"  "<<bitset<4>(mask)<<" - "<<used.SZ<<endl<<"   -> ";
                    if (used.SZ>3) continue;
                    sort(ALL(is));
                    if (is[0].x>1 || is[is.SZ-1].x<10001) continue;
                 //   Repj(is.SZ) cout<<is[j].x<<","<<is[j].y<<" "; cout<<endl;
                    bool ok=1;
                    int cnt=0;
                    Repj(is.SZ-1)
                     {
                       if (is[j].y==-1) cnt++; else cnt--;
                       if (!cnt) { ok=0; break; }
                     }
                    if (!ok) continue;
                  //  cout<<"     OK!\n";
                    ans=min(ans,__builtin_popcount(mask));
            }
           if (ans<1000000000)
             cout<<"Case #"<<i+1<<": "<<ans<<"\n";
           else
             cout<<"Case #"<<i+1<<": IMPOSSIBLE\n";
     }
    return 0;
}
