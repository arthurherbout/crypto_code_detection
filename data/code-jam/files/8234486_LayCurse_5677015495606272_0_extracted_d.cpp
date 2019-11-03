#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define mygc(c) (c)=getchar_unlocked()
#define mypc(c) putchar_unlocked(c)

#define ll long long
#define ull unsigned ll

void reader(int *x){int k,m=0;*x=0;for(;;){mygc(k);if(k=='-'){m=1;break;}if('0'<=k&&k<='9'){*x=k-'0';break;}}for(;;){mygc(k);if(k<'0'||k>'9')break;*x=(*x)*10+k-'0';}if(m)(*x)=-(*x);}
void reader(ll *x){int k,m=0;*x=0;for(;;){mygc(k);if(k=='-'){m=1;break;}if('0'<=k&&k<='9'){*x=k-'0';break;}}for(;;){mygc(k);if(k<'0'||k>'9')break;*x=(*x)*10+k-'0';}if(m)(*x)=-(*x);}
int reader(char c[]){int i,s=0;for(;;){mygc(i);if(i!=' '&&i!='\n'&&i!='\r'&&i!='\t'&&i!=EOF) break;}c[s++]=i;for(;;){mygc(i);if(i==' '||i=='\n'||i=='\r'||i=='\t'||i==EOF) break;c[s++]=i;}c[s]='\0';return s;}
template <class T, class S> void reader(T *x, S *y){reader(x);reader(y);}
template <class T, class S, class U> void reader(T *x, S *y, U *z){reader(x);reader(y);reader(z);}
template <class T, class S, class U, class V> void reader(T *x, S *y, U *z, V *w){reader(x);reader(y);reader(z);reader(w);}

void writer(int x, char c){int s=0,m=0;char f[10];if(x<0)m=1,x=-x;while(x)f[s++]=x%10,x/=10;if(!s)f[s++]=0;if(m)mypc('-');while(s--)mypc(f[s]+'0');mypc(c);}
void writer(ll x, char c){int s=0,m=0;char f[20];if(x<0)m=1,x=-x;while(x)f[s++]=x%10,x/=10;if(!s)f[s++]=0;if(m)mypc('-');while(s--)mypc(f[s]+'0');mypc(c);}
void writer(const char c[]){int i;for(i=0;c[i]!='\0';i++)mypc(c[i]);}
void writer(const char x[], char c){int i;for(i=0;x[i]!='\0';i++)mypc(x[i]);mypc(c);}
template<class T> void writerLn(T x){writer(x,'\n');}
template<class T, class S> void writerLn(T x, S y){writer(x,' ');writer(y,'\n');}
template<class T, class S, class U> void writerLn(T x, S y, U z){writer(x,' ');writer(y,' ');writer(z,'\n');}
template<class T> void writerArr(T x[], int n){int i;if(!n){mypc('\n');return;}rep(i,n-1)writer(x[i],' ');writer(x[n-1],'\n');}

#define MD 1000000007

int R, C;
int mp[10][10];
int cnt;

void dfs(int a, int b){
  int i, j, k, d, ni, nj, mn, mx;
  int dx[4] = {-1,1,0,0}, dy[4] = {0,0,-1,1};

  rep(i,R) rep(j,C){
    if(mp[i][j]==0) continue;
    mn = mx = 0;
    rep(d,4){
      ni = i + dx[d];
      nj = (j + dy[d] + C) % C;
      if(ni < 0 || ni >= R) continue;
      if(mp[ni][nj]==0) mx++;
      if(mp[i][j]==mp[ni][nj]) mn++, mx++;
    }
    if(mp[i][j] < mn || mp[i][j] > mx) return;
  }

  if(b==C){ dfs(a+1,0); return; }
  if(a==R){
    rep(i,R) rep(j,C){
      k = 0;
      rep(d,4){
        ni = i + dx[d];
        nj = (j + dy[d] + C) % C;
        if(ni < 0 || ni >= R) continue;
        if(mp[i][j]==mp[ni][nj]) k++;
      }
      if(k!=mp[i][j]) return;
    }

    {
      vector<int> now, hoge;
      rep(i,R) rep(j,C) now.push_back(mp[i][j]);
      rep(k,C){
        hoge.clear();
        rep(i,R) rep(j,C) hoge.push_back(mp[i][(j+k)%C]);
        if(hoge < now) return;
      }
    }

    puts("");
    rep(i,R) writerArr(mp[i],C);
    cnt++;
    printf("cnt %d\n",cnt);

    return;
  }

  REP(i,1,4){
    mp[a][b] = i;
    dfs(a,b+1);
    mp[a][b] = 0;
  }
}

int get_inv(ll a, int md){ll t=a,s=md,u=1,v=0,e;while(s){e=t/s;t-=e*s;u-=e*v;swap(t,s);swap(u,v);}if(u<0)u+=md;return u;}
template<class T> T GCD(T a,T b){T r; while(a){r=b; b=a; a=r%a;} return b;}
template<class T> T LCM(T a,T b){return a/GCD(a,b)*b;}

ll dp[13][120];

int main(){
  int i, j, k;
  int T, Case = 0;
  ll res, tmp;

//  R = 6;
//  C = 4;
//  dfs(0,0);
//  return 0;

  reader(&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++Case);

    reader(&R,&C);
    
    res = 1;
    if(R%3==0) res += 1;

    rep(k,13)rep(i,120) dp[k][i] = 0;
    dp[1][0] = 1;
    dp[1][2] = 1;
    
    REP(k,1,13) rep(i,110) if(dp[k][i]){
      dp[k][i+3] += dp[k][i];
      dp[k][i+3] %= MD;
      if(C%4==0){
        dp[LCM(k,4)][i+5] += 4*dp[k][i];
        dp[LCM(k,4)][i+5] %= MD;
      }
      if(C%3==0){
        dp[LCM(k,3)][i+4] += 3*dp[k][i];
        dp[LCM(k,3)][i+4] %= MD;
      }
      if(C%6==0){
        dp[LCM(k,6)][i+4] += 6*dp[k][i];
        dp[LCM(k,6)][i+4] %= MD;
      }
    }
    
    res = 0;
    
    REP(k,1,13) res += dp[k][R] * get_inv(k, MD) % MD;
    REP(k,1,13) res += dp[k][R-1] * get_inv(k, MD) % MD;

    if(C%4==0 && R>=3){
      REP(k,1,13) res += dp[k][R-3] * 4 * get_inv(LCM(k,4), MD) % MD;
    }
    
    if(C%3==0){
      REP(k,1,13) res += dp[k][R-2] * 3 * get_inv(LCM(k,3), MD) % MD;
    }
    
    if(C%6==0){
      REP(k,1,13) res += dp[k][R-2] * 6 * get_inv(LCM(k,6), MD) % MD;
    }
    
    res %= MD;
    if(res < 0) res += MD;

    writerLn(res);
  }

  return 0;
}
