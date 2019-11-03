#include<stdio.h>
#include<string.h>
#include<vector>
#include<algorithm>
using namespace std;
int n,m;
bool v[10][10];
int co[10],bes[10];
vector<vector<int> > a;
vector<int> b,c,d;
bool dfs(int i,int u){
	if(i<n){
		for(co[i]=0; co[i]<u; co[i]++)
			if(dfs(i+1,u))return true;
		return false;
	}else{
		for(int j=0; j<m; j++){
			int mask=0;
			for(int k=0; k<n; k++)
				if(v[j][k])
					mask|=(1<<co[k]);
			if(mask!=(1<<u)-1)return false;
		}
		memcpy(bes,co,sizeof(bes));
		return true;
	}
}
int main(){
	int _,t,X[10],x,y;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%d%d",&n,&m);
		a.clear();
		b.clear();
		for(int i=0; i<n; i++)b.push_back(i);
		a.push_back(b);
		for(int i=0; i<m; i++)scanf("%d",&X[i]);
		for(int i=0; i<m; i++){
			scanf("%d",&y);x=X[i]-1;y--;
			for(int i=0; i<a.size(); i++){
				int t1,t2;
				for(t1=0; t1<a[i].size(); t1++)if(a[i][t1]==x)break;
				for(t2=0; t2<a[i].size(); t2++)if(a[i][t2]==y)break;
				if(t1<a[i].size() && t2<a[i].size()){
					if(t1>t2)swap(t1,t2);
					b=a[i];
					swap(a[i],a[a.size()-1]);
					a.pop_back();
					c.clear();
					d.clear();
					for(int i=0; i<b.size(); i++){
						if(i<=t1 || i>=t2)
							c.push_back(b[i]);
						if(i>=t1 && i<=t2)
							d.push_back(b[i]);
					}
					a.push_back(c);
					a.push_back(d);
					break;
				}
			}
		}
		memset(v,0,sizeof(v));
		m=a.size();
		for(int i=0; i<m; i++)
			for(int j=0; j<a[i].size(); j++)
				v[i][a[i][j]]=true;
		int col;
		for(col=1; col<=5 && dfs(1,col); col++);
		printf("Case #%d: %d\n",t,col-1);
		for(int j=0; j<n; j++)
			printf("%d%c",bes[j]+1,j<n-1?' ':'\n');
	}
	return 0;
}
