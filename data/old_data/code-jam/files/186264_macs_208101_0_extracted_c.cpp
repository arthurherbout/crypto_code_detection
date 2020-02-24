#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <cctype>
#include <queue>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define mx 100
#define in(x) (x>=0 && x<n)
#define lim 21

struct ST
{
  int x,y,z,s;
  string str;
};

int cs,c,n,q,i,j,k,x,y,z,x2,y2,z2,s,s2,x3,y3;
string str,res;
ST st;
int dx[]={-1,0,1,0},dy[]={0,1,0,-1};
char A[lim][lim];
int D[lim][lim][2*mx];
string M[lim][lim][2*mx];
queue <ST> Q;

ST set(int x,int y,string str,int z,int s)
{
  ST st;
  
  st.x=x;
  st.y=y;
  st.str=str;
  st.z=z;
  st.s=s;
  return st;
}
int main()
{
  scanf("%d",&cs);
  for(c=1;c<=cs;c++)
  {
    scanf("%d%d",&n,&q);
	for(i=0;i<n;i++)
	{
	  scanf("%s",A[i]);
	}
	memset(D,-1,sizeof D);
	for(i=0;i<n;i++)
	  for(j=0;j<n;j++)
	    if(isdigit(A[i][j]))
		{
		  str=A[i][j];
		  st=set(i,j,str,1,A[i][j]-'0'+mx);
		  Q.push(st);
		  D[i][j][A[i][j]-'0'+mx]=1;
		  M[i][j][A[i][j]-'0'+mx]=A[i][j];
		}
	while(!Q.empty())
	{
	  x=Q.front().x;
	  y=Q.front().y;
	  str=Q.front().str;
	  z=Q.front().z;
	  s=Q.front().s;
	  Q.pop();
//	  printf("%d %d %d %s\n",x,y,s-mx,str.c_str());
	  for(i=0;i<4;i++)
	  {
	    x2=x+dx[i];
	    y2=y+dy[i];
	    if(in(x2) && in(y2))
		{
		  for(j=0;j<4;j++)
		  {
		    x3=x2+dx[j];
		    y3=y2+dy[j];
			if(in(x3) && in(y3))
			{
			  if(A[x2][y2]=='+')
			  {
			    s2=s+A[x3][y3]-'0';
			  }
			  else
			    s2=s-(A[x3][y3]-'0');
			  st=set(x3,y3,str+A[x2][y2]+A[x3][y3],z+2,s2);
			  if(D[x3][y3][s2]==-1 && s2>=0 && s2<2*mx)
			  {
			    D[x3][y3][s2]=z+2;
				M[x3][y3][s2]=str+A[x2][y2]+A[x3][y3];
				Q.push(st);
			  }
			  if(z+2==D[x3][y3][s2])
			    if(str+A[x2][y2]+A[x3][y3]<M[x3][y3][s2])
				{
			      M[x3][y3][s2]=str+A[x2][y2]+A[x3][y3];
				  Q.push(st);
				}
			}
		  }
		}
	  }
	}
	printf("Case #%d:\n",c);
	for(i=0;i<q;i++)
	{
	  scanf("%d",&x);
	  x+=mx;
	  res="";
	  for(j=0;j<n;j++)
	    for(k=0;k<n;k++)
		  if(isdigit(A[j][k]))
		    if(M[j][k][x].size())
			{
			  if(!res.size())
			    res=M[j][k][x];
			  if(M[j][k][x].size()<res.size() || (M[j][k][x].size()==res.size() && M[j][k][x]<res))
			    res=M[j][k][x];
			}
	  printf("%s\n",res.c_str());  
	}
  }  
  return 0;
}
