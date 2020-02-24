#include<stdio.h>

#define INF 1000000000

int a[5][5], c[5][5];
int n, m;
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};

int mx, my;

int Opt = 0;

void Go(int Dep)
{
	
/*	
	{
		int l1, l2;
		printf("==%d==\n",Dep);
		for(l1=0;l1<n;l1++)
		{
			for(l2=0;l2<m;l2++)
			{
				printf("%3d",a[l1][l2]);
			}
			printf("\n");
		}
		printf("====\n");
	}
*/	
	
	// check if end
	if(Opt == INF) return;

	if(Dep > Opt) Opt = Dep;
	if(a[mx][my] == 0)
	{
		return;
	}

	int l3;
	for(l3=0;l3<4;l3++)
	{
		int t1 = mx + dx[l3];
		int t2 = my + dy[l3];
		if(t1 < 0 || t2 < 0 || t1 >= n || t2 >= m) continue;
		if(a[t1][t2] != 0) break;
	}
	if(l3 == 4){ Opt = INF; return; }


	// make bakup
	int b[5][5];
	int l1, l2;
	for(l1=0;l1<n;l1++) for(l2=0;l2<m;l2++){ b[l1][l2] = a[l1][l2]; c[l1][l2] = 0; }
	for(l1=0;l1<n;l1++) for(l2=0;l2<m;l2++)
	{
		if(a[l1][l2])
		{
			if(l1 == mx && l2 == my) continue;

			int idx = -1, mv = -INF;
			for(l3=0;l3<4;l3++)
			{
				int t1 = l1 + dx[l3];
				int t2 = l2 + dy[l3];

				if(t1 < 0 || t2 < 0 || t1 >= n || t2 >= m) continue;

				if(a[t1][t2] > mv)
				{
					mv = a[t1][t2];
					idx = l3;
				}
			}
			if(idx != -1)
			{
				int t1 = l1 + dx[idx];
				int t2 = l2 + dy[idx];
				c[t1][t2] += a[l1][l2];
			}
		}
	}
	for(l1=0;l1<n;l1++)
	{
		for(l2=0;l2<m;l2++)
		{
			a[l1][l2] -= c[l1][l2];
			if(a[l1][l2] < 0) a[l1][l2] = 0;
		}
	}

	Go(Dep + 1);
	
	int c[5][5];
	for(l3=0;l3<4;l3++)
	{
		int t1 = mx + dx[l3];
		int t2 = my + dy[l3];
		if(t1 < 0 || t2 < 0 || t1 >= n || t2 >= m) continue;

		for(l1=0;l1<n;l1++) for(l2=0;l2<m;l2++) c[l1][l2] = a[l1][l2];

		int prev = a[t1][t2];
		a[t1][t2] -= a[mx][my];
		if(a[t1][t2] < 0) a[t1][t2] = 0;
		Go(Dep + 1);
		a[t1][t2] = prev;
		for(l1=0;l1<n;l1++) for(l2=0;l2<m;l2++) a[l1][l2] = c[l1][l2];
	}
	

	for(l1=0;l1<n;l1++) for(l2=0;l2<m;l2++) a[l1][l2] = b[l1][l2];
}

int main(void)
{
	int T, l0, l1, l2;

	freopen("b1.in","r",stdin);
	freopen("b1.out","w",stdout);
	//freopen("input.txt","r",stdin);

	scanf("%d",&T);
	for(l0=1;l0<=T;l0++)
	{
		scanf("%d %d %d %d",&m,&n,&my,&mx);
		mx--;
		my--;
		
		for(l1=0;l1<n;l1++)
		{
			for(l2=0;l2<m;l2++)
			{
				scanf("%d",&a[l1][l2]);
			}
		}
		Opt = 0;
		Go(0);

		printf("Case #%d: ",l0);
		if(Opt == INF) printf("forever\n");
		else printf("%d day(s)\n",Opt-1);

//		printf("--\n");
	}

	return 0;
}