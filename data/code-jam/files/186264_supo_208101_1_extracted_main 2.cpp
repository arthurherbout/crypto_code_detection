//DEDICATED TO EMMA WATSON, THE BRITISH *SUNSHINE*
#include <vector>
#include <list>
#include <map>
#include <set>
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
//#include <fstream>

#define eps 10e-10
#define INF 1000000000
#define PI 3.141592653589793238462
#define EU 2.71828182845904523536
#define sz(a) (int)a.size()
#define pb(a) push_back(a)
#define mset(a,hodnota) memset(a,hodnota,sizeof(a))
#define wh(a) a.begin(),a.end()
#define REP(i,n) for(__typeof(n) i=0;i<(n);++i)
#define REPS(i,n) for(int(i)=0;i<int(n.size());++i)
#define FOR(i,a,b) for(__typeof(b) i=(a);i<=(b);++i)
#define FORD(i,a,b) for(__typeof(a) i=(a);i>=(b);--i)
#define FORE(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)

#define SQR(a) ((a)*(a))
#define pii pair<int,int>
#define mp(a,b) make_pair(a,b)
#define fi first
#define se second
typedef long long ll;
typedef long double ld;
using namespace std;

struct state
{
  int r,c,sum;  
  string s;
  bool operator < (const state &b)const
  {
//     int res=strcmp(buf,b.buf);
//     if (res!=0)return res<0;

//     if (dlzka!=b.dlzka) return dlzka<b.dlzka;
    if (sz(s)!=sz(b.s))return sz(s)<sz(b.s);
    if (s!=b.s) return s<b.s;
    if (sum!=b.sum)return sum<b.sum;
    return mp(r,c)<mp(b.r,b.c);  
  }
};
set<state>Q;
string DP[22][22][1005];
void update(state a)
{
#define SENT 500
  if (sz(DP[a.r][a.c][a.sum+SENT])==0 || mp(sz(a.s),a.s) < mp(sz(DP[a.r][a.c][a.sum+SENT]),DP[a.r][a.c][a.sum+SENT]))
  {
    state old=a;old.s=DP[a.r][a.c][a.sum+SENT];
    Q.erase(old);
    DP[a.r][a.c][a.sum+SENT]=a.s;
    Q.insert(a);  
  }
}

int W,Qu;
int M[55][55];
bool OP[55][55];


void solve_case()
{
  cin>>W>>Qu;putchar(10);
  Q.clear();
  REP(i,W)REP(j,W)REP(k,1001)DP[i][j][k]="";
  REP(i,W)
  {
    REP(j,W)
    {
      char ch;
      cin>>ch;
      M[i][j]= ch=='-'?-1:ch=='+'?1:ch-'0';
      OP[i][j]= (ch=='-'||ch=='+');
    }
  }
/*  REP(i,W)
  {
    REP(j,W)
      cout<<M[i][j]<<" ";
    cout<<endl;
  
  }
  cout<<"------------"<<endl;
  */
  REP(i,W)
    REP(j,W)
      if (!OP[i][j])
      {
        stringstream ss;ss<<char(M[i][j]+'0');
        state cur={i,j,M[i][j],ss.str()};
//         cout<<"vkladam : "<<cur.r<<" "<<cur.c<<" "<<cur.sum<<" "<<cur.s<<endl;

        update(cur);
      }
//   int dir[12][2][2]={ {-1,0,-1,0}, {-1,0, 0,1}, {-1,0, 0,-1}, 
//                       {0,1, 0,1},  {0,1,  1,0}, {0,1, -1, 0}, 
//                       {1,0, 1,0}, {1,0, 0,1}, {1,0, 0,-1}, 
//                       {0,-1, 0,-1}, {0,-1, 1,0}, {0,-1, -1,0}};
    int dir[16][2][2]={ {-1,0,-1,0},{-1,0,1,0}, {-1,0, 0,1}, {-1,0, 0,-1}, 
                      {0,1, 0,1},{0,1, 0,-1},  {0,1,  1,0}, {0,1, -1, 0}, 
                      {1,0, 1,0},{1,0, -1,0}, {1,0, 0,1}, {1,0, 0,-1}, 
                      {0,-1, 0,-1},{0,-1, 0,1}, {0,-1, 1,0}, {0,-1, -1,0}};

///  REP(i,22)REP(j,22)REP(k,255)DP[i][j][k]="";                    
  vector<int> kver;
  set<int> kvers;
  REP(kk,Qu)
  {
    int val;
    cin>>val;
    kver.pb(val);
    kvers.insert(val);
  }

  while(sz(Q))
  {

    state top=*Q.begin();
    Q.erase(Q.begin());
//     cout<<sz(Q)<<endl;
//      cout<<top.r<<" "<<top.c<<" "<<top.sum<<" "<<top.s<<endl;
    kvers.erase(top.sum);
     if (sz(kvers)==0)break;
     if (top.sum<-490||top.sum>490)continue;
//     int sum=0,bad=0;
//     for (int i=sz(s)-1;i>=1;i-=2)
//     {
//       sum+=(s[i]-'0')*(s[i-1]=='+'?1:-1);
//       if (sum==0)bad=1;    
//     }


    REP(k,16)
    {
      int op=1;
      state n;
      n.r=top.r,n.c=top.c,n.sum=top.sum;
      n.r+=dir[k][0][0],n.c+=dir[k][0][1];
      if (n.r<0||n.r>=W||n.c<0||n.c>=W)continue;
      op*=M[n.r][n.c];
      n.r+=dir[k][1][0],n.c+=dir[k][1][1];
      if (n.r<0||n.r>=W||n.c<0||n.c>=W)continue;      
      n.sum+=op*M[n.r][n.c];
      n.s=top.s+(op==1?"+":"-")+char(M[n.r][n.c]+'0');
      update(n);          
    }
  }
  REP(kk,Qu)
  {
    int val=kver[kk];
    string ans="";
    REP(i,W)REP(j,W)      
    if (sz(DP[i][j][val+SENT])!=0)
    {
      if (sz(ans)==0)ans=DP[i][j][val+SENT];
      else if (mp(sz(ans),ans)>mp(sz(DP[i][j][val+SENT]),DP[i][j][val+SENT]))
        ans=DP[i][j][val+SENT];    
    }
    cout<<ans<<endl;      
  }
}


int cases;
int main( )
{
  scanf("%d",&cases);getchar();
  REP(ii,cases)
  {
    printf("Case #%d: ",ii+1);
    solve_case();
  }         
  return 0;
}
