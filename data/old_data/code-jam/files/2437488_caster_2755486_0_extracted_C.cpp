#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <queue>

using namespace std;

const int MAXN = 1010;

struct node
{
  int t,m,w,e,s,dt,dp,ds;
  void scan()
  {
    scanf("%d%d%d%d%d%d%d%d",&t,&m,&w,&e,&s,&dt,&dp,&ds);
  }
  bool get_nxt()
  {
    if (--m == 0) return false;
    t += dt;
    w += dp,e += dp;
    s += ds;
    return true;
  }
} p[MAXN];


const int MAXM = 2010,zero = 1000;
int _h[MAXM];
int *h = _h + zero;

typedef pair<int,int> pii;

struct cmp
{
  bool operator() (node * a,node * b)
  {
    return a->t > b->t;
  }
};

node tlst[MAXN];

const int INF = 1 << 29;
bool check_suc(const node &p)
{
  int minh = INF;
  for(int i = p.w; i < p.e; i++)
    minh = min(h[i],minh);

  //printf("%d %d %d\n",p.w,p.e,minh);
  return p.s > minh;
}

void apply_raise(const node &p)
{
  for(int i = p.w; i < p.e; i++) 
    {
      h[i] = max(h[i],p.s);
      //printf("A%d %d\n",i,h[i]);
    }
}

int main()
{
  int tn;
  scanf("%d",&tn);
  for(int t = 1; t <= tn; t++)
    {
      priority_queue<node*,vector<node*>,cmp >  que;

      int n;
      scanf("%d",&n);
      for(int i = 0; i < n; i++) 
	{
	  p[i].scan();
	  que.push(p + i);
	}

      memset(_h,0,sizeof(_h));
      
      int ans = 0;
      while(!que.empty())
	{
	  int curT = que.top()->t,tot = 0; 
	  while(!que.empty() && que.top()->t == curT)
	    {
	      node *u = que.top();
	      que.pop();

	      ans += check_suc(*u);
	      tlst[tot++] = *u;

	      if (u->get_nxt()) que.push(u);
	    }

	  for(int i = 0; i < tot; i++) apply_raise(tlst[i]);
	}

      printf("Case #%d: %d\n",t,ans);
    }
}
