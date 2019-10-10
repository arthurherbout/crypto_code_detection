#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
using namespace std;

#define CLEAR(t) memset((t),0,sizeof(t))
#define FOR(i,a,b) for(__typeof(a)i=(a);i<=(b);++i)
#define FORD(i,a,b) for(__typeof(a)i=(a);i>=(b);--i)
#define REP(i,n) for(__typeof(n)i=0;i<(n);++i)
#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
#define PB(x) push_back(x)
#define MP(x,y) make_pair(x,y)

vector<string> split(const string& s, const string& delim=" ")
{ vector<string> res; string t; for(unsigned int i=0;i<s.length();i++) { if(delim.find(s[i])!=string::npos) { if(!t.empty()) { res.PB(t); t=""; } } else t+=s[i]; } if(!t.empty()) res.PB(t); return res; }
vector<int> splitint(const string& s, const string& delim=" ") { vector<string> vs=split(s); vector<int> res; for(unsigned int i=0;i<vs.size();i++) res.PB(atoi(vs[i].c_str())); return res; }

struct vec {
  double x,y;
};

struct mat {
  double a,b,c,d;
};

mat v2m(vec u, vec v)
{
  mat m;
  m.a=u.x; m.b=v.x; m.c=u.y; m.d=v.y;
  return m;
}

vec mtv(mat m, vec v)
{
  vec r;
  r.x=m.a*v.x+m.b*v.y;
  r.y=m.c*v.x+m.d*v.y;
  return r;
}

mat mtm(mat m, mat n)
{
  mat r;
  r.a=m.a*n.a+m.b*n.c;
  r.b=m.a*n.b+m.b*n.d;
  r.c=m.c*n.a+m.d*n.c;
  r.d=m.c*n.b+m.d*n.d;
  return r;
}

mat inv(mat m)
{
  mat r;
  double det=m.a*m.d-m.b*m.c;
  r.a=m.d/det;
  r.b=-m.b/det;
  r.c=-m.c/det;
  r.d=m.a/det;
  return r;
}

void _case(int casenum)
{
  double xx,yy;
  double x1,y1,x2,y2,x3,y3;
  vec u,v,uu,vv;
  scanf("%lf%lf%lf%lf%lf%lf",&x1,&y1,&x2,&y2,&x3,&y3);
  u.x=x2-x1; u.y=y2-y1;
  v.x=x3-x1; v.y=y3-y1;
  xx=x1; yy=y1;
  scanf("%lf%lf%lf%lf%lf%lf",&x1,&y1,&x2,&y2,&x3,&y3);
  uu.x=x2-x1; uu.y=y2-y1;
  vv.x=x3-x1; vv.y=y3-y1;

//  printf("u=(%.3lf,%.3lf), v=(%.3lf,%.3lf)\n",u.x,u.y,v.x,v.y);
//  printf("uu=(%.3lf,%.3lf), vv=(%.3lf,%.3lf)\n",uu.x,uu.y,vv.x,vv.y);
  
  vec b;
  b.x=x1-xx; b.y=y1-yy;
  mat uv=v2m(u,v);
  mat uuvv=v2m(uu,vv);
  mat iuv=inv(uv);
  mat aa=mtm(uuvv,iuv);

//  printf("uv=[[%.3lf,%.3lf],[%.3lf,%.3lf]]\n",uv.a,uv.b,uv.c,uv.d);
//  printf("iuv=[[%.3lf,%.3lf],[%.3lf,%.3lf]]\n",iuv.a,iuv.b,iuv.c,iuv.d);
//  printf("uuvv=[[%.3lf,%.3lf],[%.3lf,%.3lf]]\n",uuvv.a,uuvv.b,uuvv.c,uuvv.d);
//  printf("aa=[[%.3lf,%.3lf],[%.3lf,%.3lf]]\n",aa.a,aa.b,aa.c,aa.d);

  vec uuu=mtv(aa,u);
  vec vvv=mtv(aa,v);
//  printf("aa*u=(%.3lf,%.3lf), aa*v=(%.3lf,%.3lf)\n",uuu.x,uuu.y,vvv.x,vvv.y);

  aa.a=1-aa.a;
  aa.b=-aa.b;
  aa.c=-aa.c;
  aa.d=1-aa.d;

  vec res=mtv(inv(aa),b);
  printf("Case #%d: %.7lf %.7lf\n",casenum,res.x+xx,res.y+yy);
}

int main()
{
  int n;
  scanf("%d ",&n);
  FOR(i,1,n) _case(i);
  return 0;
}
