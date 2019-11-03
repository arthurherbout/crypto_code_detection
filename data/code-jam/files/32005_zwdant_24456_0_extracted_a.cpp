#include<stdio.h>
#include<string.h>

const int maxn = 1000;
int n;
struct Record{
	int w, h;
	int k;
}d[maxn];
int w1, w2, h1, h2;
int w3, w4, h3,h4;
int nobird;

int main() {
	int i,j;
	int cs,step;
	char s[100];
	scanf("%d",&cs);
	for(step=1;step<=cs;step++)
	{
		printf("Case #%d:\n", step);
		scanf("%d",&n);
		w1 = h1 = -1;
		w2 = h2 = -1;
		for(i=0;i<n;i++){
			scanf("%d%d",&d[i].w, &d[i].h);
			gets(s);
			if(s[1]=='B') d[i].k = 1; 
			else if(s[1]=='N') d[i].k = 0;
			else { printf("Error"); return 0;}
			//printf("%d %d %d\n", d[i].w, d[i].h, d[i].k);
			if(d[i].k)
			{
				if(w1 == -1) w1 = w2 = d[i].w;
				else {
					if(d[i].w<w1) w1=d[i].w;
					if(d[i].w>w2) w2=d[i].w;
				}
				if(h1 == -1) h1 = h2 = d[i].h;
				else {
					if(d[i].h<h1) h1=d[i].h;
					if(d[i].h>h2) h2=d[i].h;
				}
			}
		}
		w3 = 1; w4 = 1000000;
		h3 = 1; h4 = 1000000;
		if(w1!=-1){
			for(i=0;i<n;i++)if(d[i].k==0){
				if(d[i].w>=w1 && d[i].w<=w2){
					if(d[i].h<h1) if(d[i].h+1>h3) h3 = d[i].h+1;
					if(d[i].h>h2) if(d[i].h-1<h4) h4 = d[i].h-1;
				}
				else if(d[i].h>=h1 && d[i].h<=h2)
				{
					if(d[i].w<w1) if(d[i].w+1>w3) w3=d[i].w+1;
					if(d[i].w>w2) if(d[i].w-1<w4) w4=d[i].w-1;
				}
				else {
					//
				}
			}
		}
		int w, h, m;
		scanf("%d",&m);
		
		//printf("%d %d, %d %d\n", w1, w2, h1, h2);
		for(i=0;i<m;i++)
		{
			scanf("%d%d",&w,&h);
			int k = -1;
			//printf("%d %d\n", w, h);
			if(w>=w1 && w<=w2 && h>=h1 && h<=h2) k = 1;
			if(w<w3 || w>w4 || h<h3 || h>h4) k = 0;

			if(k==-1)
			{
				for(j=0;j<n && k==-1;j++)if(d[j].k==0 && w==d[j].w && h==d[j].h) k = 0;
				
				for(j=0;j<n && k==-1 && w1!=-1;j++)if(d[j].k==0)
				{
					int k1=0, k2=0;
					if(d[j].w<w1 && w<=d[j].w) k1 = 1;
					if(d[j].w>w2 && w>=d[j].w) k1 = 1;
					if(d[j].h<h1 && h<=d[j].h) k2 = 1;
					if(d[j].h>h2 && h>=d[j].h) k2 = 1;
					if(k1 && k2) k = 0;
				}
			}
			if(k==1)printf("BIRD\n");
			else if(k==0) printf("NOT BIRD\n");
			else printf("UNKNOWN\n");
		}
	}
	return 0;
}
