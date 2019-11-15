#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<vector>
#define x first
#define y second
using namespace std;
int n;
int x[99],y[99],id[99],o[99];


#define Point pair<int,int>
int operator *(const Point&x,const Point&y)
{
	return x.second*y.first-x.first*y.second;
}
Point operator -(const Point&x,const Point&y)
{
	static Point z;
	z.first=x.first-y.first,z.second=x.second-y.second;
	return z;
}
int operator%(Point a,Point b)
{
	return a.x*b.x+a.y*b.y;
}

bool intersect(Point A,Point B,Point C,Point D)
{
	int d1=(B-A)*(C-A);
	int d2=(B-A)*(D-A);
	int d3=(D-C)*(A-C);
	int d4=(D-C)*(B-C);
	if((d1<0 && d2>0 || d1>0 && d2<0) &&
	   (d3<0 && d4>0 || d3>0 && d4<0))
	return 1;
	return d1==0 && (A-C)%(B-C)<=0
	    || d2==0 && (A-D)%(B-D)<=0
	    || d3==0 && (C-A)%(D-A)<=0
	    || d4==0 && (C-B)%(D-B)<=0;
}

int calc(vector<Point > v)
{
	int t=v.size(),n=0;
	sort(v.begin(),v.end());
	vector<Point > a(t+5);
	bool flag[99];
	int now[99];
	memset(flag,0,sizeof(flag));
	a[n++]=v[0];
	for(int i=2; i<t; i++)
	{
		while(n>1 && (v[i]-a[n-2])*(a[n-1]-a[n-2])<=0)
		{
			n--;
			flag[now[n]]=false;
		}
		a[n]=v[i];
		now[n]=i;
		flag[i]=true;
		n++;
	}
	int w=n-1;
	for(int i=t-1; i>=0; i--)
	{
		if(flag[i])continue;
		while(n-w>1 && (v[i]-a[n-2])*(a[n-1]-a[n-2])<=0)
		n--;
		a[n++]=v[i];
	}
	n--;
	int ans=0;
	for(int i=2; i<n; i++)
		ans+=(a[i-1]-a[0])*(a[i]-a[0]);
	return labs(ans);
}

Point a[99];

int main(){
	int _;
	scanf("%d",&_);
	for(int t=1; t<=_; t++){
		scanf("%d",&n);
		vector<Point> v;
		for(int i=0; i<n; i++){
			scanf("%d%d",&x[i],&y[i]);
			v.push_back(make_pair(x[i],y[i]));
		}
		int ans=calc(v);

		memset(o,-1,sizeof(o));
		int nowans=0;
		for(int i=0; i<n; i++)
			id[i]=i;
		do{
			for(int i=0; i<n; i++)
				a[i]=make_pair(x[id[i]],y[id[i]]);
			bool flag=true;
			for(int i=0; i<n && flag; i++)
				for(int j=0; j<n && flag; j++){
					Point A=a[i],B=a[(i+1)%n],C=a[j],D=a[(j+1)%n];
					if(A==C || A==D || B==C || B==D)continue;
					if(intersect(a[i],a[(i+1)%n],a[j],a[(j+1)%n]))
						flag=false;
				}
			if(flag){
				int myans=0;
				for(int i=2; i<n; i++)
					myans+=(a[i-1]-a[0])*(a[i]-a[0]);
				myans=labs(myans);
				if(myans*2>ans){
					if(myans>nowans){
						nowans=myans;
						memcpy(o,id,sizeof(id));
					}
				}
			}

		}while(next_permutation(id+1,id+n));

		printf("Case #%d:",t);
		for(int i=0; i<n; i++)
			printf(" %d",o[i]);
		puts("");
	}
	return 0;
}