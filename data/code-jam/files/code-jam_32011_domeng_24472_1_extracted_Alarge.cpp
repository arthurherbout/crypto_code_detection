#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;
struct tnode
{
	int sx,sy,lx,ly,cnt;
	tnode *L,*R;
	tnode(int _sx,int _sy,int _lx,int _ly):sx(_sx),sy(_sy),lx(_lx),ly(_ly) { L=R=NULL;cnt=0; }
	~tnode()
	{
		if (L) delete L;
		if (R) delete R;
	}
};
int IDX[5000],N,REQ[5000][3];
int good(tnode *n,int y,int x)
{
	if (n->sy<=y && n->ly>=y && n->sx<=x && n->lx>=x) return 1;
	return 0;
}
void addpoint(tnode *n,int y,int x)
{
	int lx=n->lx;
	int ly=n->ly;
	int sx=n->sx;
	int sy=n->sy;
	int dx=lx-sx, dy=n->ly-sy;
	(n->cnt)++;
	if (dx==0 && dy==0) return; //done : leaf
	if (dx<dy) // horizon cut : dy dec
	{
		int m=dy/2;
		if (n->L==NULL)
		{
			n->L=new tnode(sx,sy,lx,sy+m);
			n->R=new tnode(sx,sy+m+1,lx,ly);
		}
		if ( good(n->L,y,x) ) addpoint(n->L,y,x);
		else addpoint(n->R,y,x);
	}
	else //vertical cut : dx dec
	{
		int m=dx/2;
		if (n->L==NULL)
		{
			n->L=new tnode(sx,sy,sx+m,ly);
			n->R=new tnode(sx+m+1,sy,lx,ly);
		}
		if ( good(n->L,y,x) ) addpoint(n->L,y,x);
		else addpoint(n->R,y,x);
	}
}
int cntpoint(tnode *n,int y,int x)
{
	int lx=n->lx;
	int ly=n->ly;
	int sx=n->sx;
	int sy=n->sy;
	if (sx>x) return 0;
	if (sy>y) return 0;
	if (ly<=y && lx<=x) return n->cnt;
	if (n->L==NULL) return 0;
	return cntpoint(n->L,y,x)+cntpoint(n->R,y,x);
}
int stf(int a,int b)
{
	return REQ[a][0]<REQ[b][0];
}
int main()
{
	srand((unsigned)time(NULL));
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
	int kase,T;
	scanf("%d",&T);
	//T=0;//rand();
	for (kase=1;kase<=T;kase++)
	{
		fprintf(stderr,"%d\n",kase);
		scanf("%d",&N);
		int q,w;
		for (q=0;q<N;q++)
			for (w=0;w<3;w++) scanf("%d",REQ[q]+w);
		for (q=0;q<N;q++) IDX[q]=q;
		tnode *ROOT=new tnode(0,0,10000,10000);
		sort(IDX,IDX+N,stf);
		int ret=0;
		for (q=0;q<N;q++) 
		{
			//ADD
			int i=IDX[q];
			addpoint( ROOT, REQ[i][1] , REQ[i][2] );
			//AH
			int AH=REQ[i][0]; //A^ = REQ[i][0]
			for (w=0;w<N;w++)
			{
				//B^ = REQ[w][1]
				int BH=REQ[w][1];
				int CH=10000-AH-BH;//C^
				if (CH<0) continue;
				int cnt=cntpoint( ROOT,BH,CH);
				if (ret<cnt) ret=cnt;
			}
		}
		delete ROOT;
		printf("Case #%d: %d\n",kase,ret);
	}
	return 0;
}