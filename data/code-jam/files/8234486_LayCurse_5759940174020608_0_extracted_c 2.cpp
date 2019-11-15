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

template<class T>
struct ullHash{
  ull *key; T *val; unsigned memory, size, mask;

  void clear(){memset(val,0,size*sizeof(T));}
  void clear(int sz){size=1;while(size<sz)size*=2;mask=size-1;clear();}
  void init(int mem, int sz){memory=mem;size=1;while(size<sz)size*=2;mask=size-1;if(memory<size)memory=size;key=(ull*)malloc(memory*sizeof(ull));val=(T*)malloc(memory*sizeof(T));if(size)clear();}
  int function(ull a){return (((a*123456789789ULL)>>32)*987654321321ULL)&mask;}
  T get(ull a){int i=function(a);for(;key[i]!=a&&val[i]!=0;)i=(i+1)&mask;if(key[i]!=a) return 0;return val[i];}
  void set(ull a, T v){int i=function(a);for(;key[i]!=a&&val[i]!=0;)i=(i+1)&mask;key[i]=a;val[i]=v;}
  T increase(ull a, T v = 1){int i=function(a);for(;key[i]!=a&&val[i]!=0;)i=(i+1)&mask;key[i]=a;return val[i]+=v;}
};

#define MD 1000000007

int node;
char buf[1000];
ullHash<int> hash;

int myreader(string &str){
  int i, len = 0;
  ull hs = 0;

  for(;;){
    if(str.size()==0 || str[0]==' '){
      if(str[0]==' ') str = str.substr(1);
      break;
    }
    buf[len++] = str[0];
    str = str.substr(1);
  }

  rep(i,len) hs = hs * 1234123 + buf[i];
  i = hash.get(hs) - 1;
  if(i==-1){
    node++;
    hash.set(hs, node);
    return node-1;
  }
  return i;
}

template<class T, class S>
struct maxflow{
  int node, st, ed, *es, *emem, **edge, **rev, *level, *queue;
  T **flow, eps;

  void malloc(int N){int i;es=(int*)std::malloc(N*sizeof(int));emem=(int*)std::malloc(N*sizeof(int));level=(int*)std::malloc(N*sizeof(int));queue=(int*)std::malloc(N*sizeof(int));edge=(int**)std::malloc(N*sizeof(int*));rev=(int**)std::malloc(N*sizeof(int*));flow=(T**)std::malloc(N*sizeof(T*));rep(i,N)emem[i]=0,edge[i]=rev[i]=NULL,flow[i]=NULL;}
  void* malloc(int N, void* mem){int i;es=(int*)mem;emem=es+N;level=emem+N;queue=level+N;edge=(int**)(queue+N);rev=edge+N;flow=(T**)(rev+N);rep(i,N)emem[i]=0,edge[i]=rev[i]=NULL,flow[i]=NULL;return(void*)(flow+N);}
  void levelize(void){int i,j,k,t,s=0,e=1;rep(i,node)level[i]=-1;level[st]=0;queue[0]=st;while(s!=e){i=queue[s++];t=level[i]+1;rep(j,es[i])if(flow[i][j]>eps){k=edge[i][j];if(level[k]!=-1)continue;level[k]=t;queue[e++]=k;if(k==ed)return;}}}
  S pushflow(int i, S lim){int j,k,l;S s,t,r=0;if(i==ed)return lim;rep(j,es[i])if(flow[i][j]>eps){k=edge[i][j];if(level[k]!=level[i]+1)continue;s=min(lim,(S)flow[i][j]);t=pushflow(k,s);if(!t) continue;r+=t;lim-=t;l=rev[i][j];flow[i][j]-=t;flow[k][l]+=t;if(!lim)break;}if(lim)level[i]=-1;return r;}
  S solve(int st_, int ed_){S r=0;st=st_;ed=ed_;for(;;){levelize();if(level[ed]==-1)break;r+=pushflow(st,numeric_limits<S>::max());}return r;}
  void init(int N){int i;node=N;rep(i,N)es[i]=0;eps=(T)1e-9;}
  void memoryExpand(int i, int sz){if(sz<=emem[i])return;sz=max(sz,max(3,emem[i]*2));emem[i]=sz;edge[i]=(int*)realloc(edge[i],sz*sizeof(int));rev[i]=(int*)realloc(rev[i],sz*sizeof(int));flow[i]=(T*)realloc(flow[i],sz*sizeof(T));}
  void addEdge(int n1, int n2, T f1, T f2){int s1=es[n1]++,s2=es[n2]++;if(s1>=emem[n1])memoryExpand(n1,es[n1]);if(s2>=emem[n2])memoryExpand(n2,es[n2]);edge[n1][s1]=n2;edge[n2][s2]=n1;flow[n1][s1]=f1;flow[n2][s2]=f2;rev[n1][s1]=s2;rev[n2][s2]=s1;}
  void addEdgeAdv(int n1, int n2, T f1, T f2){int s1=es[n1]++,s2=es[n2]++;edge[n1][s1]=n2;edge[n2][s2]=n1;flow[n1][s1]=f1;flow[n2][s2]=f2;rev[n1][s1]=s2;rev[n2][s2]=s1;}
  void setGraph(int N, int M, int n1[], int n2[], int f1[], int f2[]){int i;node=N;rep(i,N)es[i]=0;rep(i,M)es[n1[i]]++,es[n2[i]]++;rep(i,N)memoryExpand(i,es[i]);rep(i,N)es[i]=0;rep(i,M)addEdgeAdv(n1[i],n2[i],f1[i],f2[i]);eps=(T)1e-9;}
  void* setGraph(int N, int M, int n1[], int n2[], T f1[], T f2[], void *mem){int i,j,k;node=N;rep(i,N)es[i]=emem[i]=0;rep(i,M)es[n1[i]]++,es[n2[i]]++;edge[0]=(int*)mem;REP(i,1,N)edge[i]=edge[i-1]+es[i-1];rev[0]=edge[N-1]+es[N-1];REP(i,1,N)rev[i]=rev[i-1]+es[i-1];flow[0]=(T*)(rev[N-1]+es[N-1]);REP(i,1,N)flow[i]=flow[i-1]+es[i-1];mem=(void*)(flow[N-1]+es[N-1]);rep(i,N)es[i]=0;rep(i,M)addEdgeAdv(n1[i],n2[i],f1[i],f2[i]);eps=(T)1e-9;return mem;}
};


int N;
int in[300][2000], ins[300];
int edge[5000][5000];

int ok[5000];

int main(){
  int i, j, k, mask;
  int T, C = 0;
  char buf[100000];
  string str;
  maxflow<int,int> flow;
  int res;

  int nd, st, ed;

  hash.init(10000, 10000);

  flow.malloc(10000);

  reader(&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++C);

    node = 0;
    hash.clear();

    reader(&N);

    rep(i,N){
      gets(buf);
      str = buf;
      ins[i] = 0;
      while(str.size()){
        k = myreader(str);
        in[i][ins[i]++] = k;
      }
    }

    nd = node;
    st = nd++;
    ed = nd++;
    flow.init(nd);

/*    rep(i,node) rep(j,node) edge[i][j] = 0;
    REP(k,2,N) rep(i,ins[k]) rep(j,ins[k]) edge[in[k][i]][in[k][j]] = 1;
    rep(k,node) rep(i,node) REP(j,i+1,node) if(edge[i][k] && edge[k][j]) edge[i][j] = edge[j][i] = 1;

    rep(i,ins[0]) flow.addEdge(st, in[0][i], 1, 0);
    rep(i,ins[1]) flow.addEdge(in[1][i], ed, 1, 0);
    rep(i,ins[0]) rep(j,ins[1]) if(edge[in[0][i]][in[1][j]]) flow.addEdge(in[0][i], in[1][j], 1, 0);

    k = flow.solve(st, ed);
    writerLn(k);*/

    res = 1000000;
    rep(mask,1<<N){
      if(mask & (1<<0)) continue;
      if(!(mask & (1<<1))) continue;

      rep(i,node) ok[i] = 0;
      rep(i,N) rep(j,ins[i]){
        if(mask&1<<i) ok[in[i][j]] |= 1;
        else          ok[in[i][j]] |= 2;
      }

      k = 0;
      rep(i,node) if(ok[i]==3) k++;
      res = min(res, k);
    }

    writerLn(res);
  }

  return 0;
}
