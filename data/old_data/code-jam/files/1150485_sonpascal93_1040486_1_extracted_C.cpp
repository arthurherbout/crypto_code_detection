#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

#define maxn 2011

using namespace std;

int test,n,m,k,res;
bool ok;
int a[maxn];
int b[maxn];
struct list {
	int n;
	int info[maxn];
	bool use[maxn];
};
list c[maxn];
int color[maxn];
bool use[maxn];
int ind[maxn];

void input(){
	int i;
	scanf("%d%d",&n,&m);
	for (i=1;i<=m;i++) scanf("%d",&a[i]);
	for (i=1;i<=m;i++) scanf("%d",&b[i]);
}

void update(list t,int a,int b,list &t1,list &t2){
	int i,u,v;
	memset(t1.use,false,sizeof(t1.use));
	memset(t2.use,false,sizeof(t2.use));
	for (i=1;i<=t.n;i++){
		if (t.info[i]==a) u=i;
		if (t.info[i]==b) v=i;
	}
	if (u>v) swap(u,v);
	t1.n=0;
	for (i=u;i<=v;i++){
		t1.n++;
		t1.info[t1.n]=t.info[i];
		t1.use[t.info[i]]=true;
	}
	t2.n=0;
	for (i=u;i>=1;i--){
		t2.n++;
		t2.info[t2.n]=t.info[i];
		t2.use[t.info[i]]=true;
	}
	for (i=t.n;i>=v;i--){
		t2.n++;
		t2.info[t2.n]=t.info[i];
		t2.use[t.info[i]]=true;
	}
}

void init(){
	int i,j;
	list t1,t2;
	k=1;
	c[1].n=n;
	for (i=1;i<=n;i++){
		c[1].info[i]=i;
		c[1].use[i]=true;
	}
	for (i=1;i<=m;i++)
		for (j=1;j<=k;j++)
			if ((c[j].use[a[i]])&&(c[j].use[b[i]])){
				update(c[j],a[i],b[i],t1,t2);
				c[j]=t1;
				k++;
				c[k]=t2;
				break;
			}
	res=n;
	for (i=1;i<=k;i++) res=min(res,c[i].n);
}

void update(){
	int i,j,v,w;
	memset(use,false,sizeof(use));
	for (i=1;i<=k;i++){
		w=0;
		for (j=1;j<=c[i].n;j++){
			v=c[i].info[j];
			if (!use[color[v]]){
				use[color[v]]=true;
				w++;
			}
		}
		if (w<res) return;
		for (j=1;j<=c[i].n;j++){
			v=c[i].info[j];
			use[color[v]]=false;
		}
	}
	ok=true;
	for (i=1;i<=n;i++) cout<<color[i]<<" ";
	cout<<"\n";
}

void visit(int i){
	int j;
	for (j=1;j<=res;j++){
		color[i]=j;
		if (i<n) visit(i+1); else update();
		if (ok) break;
	}
}

void process1(){
	ok=false;
	visit(1);
}

void Quicksort(int L,int H){
	int i,j,pivot;
	if (L>=H) return;
	pivot=ind[rand()%(H-L+1)+L];
	i=L;
	j=H;
	while (i<=j){
		while (c[ind[i]].n<c[pivot].n) i++;
		while (c[ind[j]].n>c[pivot].n) j--;
		if (i<=j){
			if (i<j) swap(ind[i],ind[j]);
			i++;
			j--;
		}
	}
	Quicksort(L,j);
	Quicksort(i,H);
}

void process2(){
	int i,j,v,w,t;
	for (i=1;i<=k;i++) ind[i]=i;
	Quicksort(1,k);
	memset(color,0,sizeof(color));
	for (t=1;t<=k;t++){
		i=ind[t];
		memset(use,false,sizeof(use));
		for (j=1;j<=c[i].n;j++){
			v=c[i].info[j];
			use[color[v]]=true;
		}
		for (j=1;j<=c[i].n;j++){
			v=c[i].info[j];
			if (color[v]!=0) continue;
			w=1;
			while (w<=res){
				if (!use[w]){
					color[v]=w;
					use[w]=true;
					break;
				}
				if (w==res){
					color[v]=w;
					use[w]=true;
					break;
				}
				w++;
			}
		}
	}
	for (i=1;i<=n;i++) cout<<color[i]<<" ";
	cout<<"\n";
}

int main(){
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	scanf("%d",&test);
	int i;
	for (i=1;i<=test;i++){
		input();
		init();
		cout<<"Case #"<<i<<": "<<res<<"\n";
		//process1();
		process2();
	}
}
