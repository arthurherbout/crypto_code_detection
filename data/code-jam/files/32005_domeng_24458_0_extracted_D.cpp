/*for smaller data*/
#include <algorithm>
#include <vector>
#include <cstdio>
#include <map>
using namespace std;
int N[2];
vector<int>adj[2][100];
struct quad
{
	int a,b,c,d;
	quad(int _a,int _b,int _c,int _d)  {a=_a,b=_b,c=_c,d=_d;}
};
bool operator<(quad u,quad v)
{
	if (u.a!=v.a) return u.a<v.a;
	if (u.b!=v.b) return u.b<v.b;
	if (u.c!=v.c) return u.c<v.c;
	return u.d<v.d;
}
map<quad,bool>M;
int TST[200][100][100],TOP;
bool solve(quad st)
{
	if (M.find(st)==M.end()) //(a,b) >= (c,d)
	{
		if (adj[0][st.b].size()<adj[1][st.d].size()) return M[st]=false;
		int i,j,u,v,my=TOP++;
		int n=adj[0][st.b].size(),m=adj[1][st.d].size();
		for (i=0;i<n;i++) for (j=0;j<m;j++) 
		{
			u=adj[0][st.b][i],v=adj[1][st.d][j];
			TST[my][i][j]=0;
			if (u==st.a || v==st.c) continue;
			if (solve(quad(st.b,u,st.d,v))) TST[my][i][j]=1;
		}
		//find cover
		int len[100],fact=1;
		for (i=0;i<n;i++) {len[i]=i;fact*=(i+1);}
		for (i=0;i<fact;i++)
		{
			for (j=0;j<m;j++) 
			{
				if (adj[1][st.d][j]==st.c) continue;
				if (my>=200)
				{
					fprintf(stderr,"%d\n",my);
				}
				if (TST[my][len[j]][j]==0) break;
			}
			if (j>=m) 
			{
				TOP--;
				return M[st]=true;
			}
			next_permutation(len,len+n);
		}
		TOP--;
		return M[st]=false;
	}
	return M[st];
}
int main()
{
	int t,T,q,w,e,r,i;
	freopen("D-small-attempt1.in","r",stdin);
	freopen("doutput.txt","w",stdout);
	scanf("%d",&T);
	for (t=1;t<=T;t++)
	{
		fprintf(stderr,"%d %d\n",t,TOP);
		for (i=0;i<=1;i++)
		{
			scanf("%d",N+i);
			for (q=0;q<N[i];q++) adj[i][q].clear();
			for (q=0;q+1<N[i];q++)
			{
				int u,v;
				scanf("%d %d",&u,&v);
				u--;v--;
				adj[i][u].push_back(v);
				adj[i][v].push_back(u);
			}
		}
		M.clear();
		bool flag=false;
		
		for (q=0;q<N[0];q++) for (w=0;w<N[1];w++) 
			if (solve(quad(-1,q,-1,w)))
				flag=true;
		printf("Case #%d: ",t);
		if (flag) printf("YES\n");
		else printf("NO\n");
			
	}
	return 0;
}