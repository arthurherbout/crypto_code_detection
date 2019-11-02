#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
#define MAXN 40
double eps=1e-6;
double pi=acos(-1);
int n;
int x[MAXN],y[MAXN],r[MAXN];

double dist(int i,int j)
{
	int dx=x[i]-x[j];
	int dy=y[i]-y[j];
	return sqrt(dx*dx+dy*dy);
}

double angle_difference(double u,double v)
{
	double ans=fabs(u-v);
	return min(ans,pi*2-ans);
}

double alpha[MAXN],beta[MAXN];
double theta[MAXN],phi[MAXN];

bool adj[MAXN*2][MAXN*2];
bool dfs_flag[MAXN*2];

bool dfs(int u,int w)
{
	dfs_flag[u]=false;
	if (u==w) return true;
	for (int v=0;v<n*2;v++)
		if (adj[u][v] && dfs_flag[v] && dfs(v,w))
			return true;
	return false;
}

int t;
int main()
{
	cout.setf(ios::fixed);
	cout.precision(10);
	cin>>t;
	for (int caseI=1;caseI<=t;caseI++)
	{
		cin>>n;
		double minr=0;
		double maxr=0;
		for (int i=0;i<n;i++)
		{
			cin>>x[i]>>y[i]>>r[i];
			if (r[i]>minr) minr=r[i];
		}
		for (int i=0;i<n;i++)
			for (int j=i+1;j<n;j++)
				maxr=max(maxr,(dist(i,j)+r[i]+r[j])*0.5);
		while (maxr-minr>eps)
		{
			double R=(maxr+minr)/2;
//			cerr<<R<<endl;
			bool flag=false;
			for (int a=0;a<n && !flag;a++)
				for (int b=a+1;b<n && !flag;b++)
				{
					memset(adj,false,sizeof(adj));
					for (int i=0;i<n;i++)
					{
						if (i==a || i==b) continue;
						alpha[i]=atan2(y[i]-y[a],x[i]-x[a]);
						beta[i]=atan2(y[i]-y[b],x[i]-x[b]);
						double d,D;
						d=dist(i,a);
						D=R*2-r[a];
						if (d+r[i]<D+eps)
						{
							if (fabs(d+r[i]-D)<eps) theta[i]=0;
							else
							{
								double c=((R-r[a])*(R-r[a])-(R-r[i])*(R-r[i])+1)/(2*(R-r[a]));
								theta[i]=acos(c);
							}
						}
						else
						{
							adj[i*2][i*2+1]=true;
							theta[i]=-1;
						}
						d=dist(i,b);
						D=R*2-r[b];
						if (d+r[i]<D+eps)
						{
							if (fabs(d+r[i]-D)<eps) phi[i]=0;
							else
							{
								double c=((R-r[b])*(R-r[b])-(R-r[i])*(R-r[i])+1)/(2*(R-r[b]));
								theta[i]=acos(c);
							}
						}
						else
						{
							adj[i*2+1][i*2]=true;
							phi[i]=-1;
						}
//						cerr<<i<<' '<<alpha[i]<<' '<<theta[i]<<' '<<beta[i]<<' '<<phi[i]<<endl;
					}
					for (int i=0;i<n;i++)
					{
						if (i==a || i==b) continue;
						for (int j=i+1;j<n;j++)
						{
							if (j==a || j==b) continue;
							double gama=angle_difference(alpha[i],alpha[j]);
							if (theta[i]>=0 && theta[j]>=0 && gama>theta[i]+theta[j]+eps)
							{
								adj[i*2][j*2+1]=true;
								adj[j*2][i*2+1]=true;
							}
							gama=angle_difference(beta[i],beta[j]);
							if (phi[i]>=0 && phi[j]>=0 && gama>phi[i]+phi[j]+eps)
							{
								adj[i*2+1][j*2]=true;
								adj[j*2+1][i*2]=true;
							}
						}
					}
/*					for (int i=0;i<n;i++)
					{
						for (int j=0;j<n;j++)
							cerr<<adj[i][j]<<' ';
						cerr<<endl;
					}*/
					bool succ=true;
					for (int i=0;i<n && succ;i++)
					{
						memset(dfs_flag,true,sizeof(dfs_flag));
						if (!dfs(i*2,i*2+1)) continue;
						memset(dfs_flag,true,sizeof(dfs_flag));
						if (!dfs(i*2+1,i*2)) continue;
						succ=false;
					}
					if (succ) flag=true;
				}
			if (flag) maxr=R;
			else minr=R;
		}
		cout<<"Case #"<<caseI<<": "<<minr<<endl;
	}
	return 0;
}

