#include <stdio.h>
char a[128][128];
char b[128][128];
int al[128];
int bl[128];
int cl[128];
int dl[128];
#define abs(x) ((x)<0?-(x):(x))
int main(void){
	int Ti,Tn,r,n,i,j,k,u,v,w;
	scanf("%d",&Tn);
	for(Ti=1;Ti<=Tn;Ti++){
		printf("Case #%d: ",Ti);
		scanf("%d",&n);
		for(i=0;i<n;i++){
			scanf("%s",a[i]);
			al[i]=0;
			bl[i]=0;
			b[i][bl[i]++]=a[i][al[i]];
			while(a[i][++al[i]]){
				if(b[i][bl[i]-1]==a[i][al[i]])continue;
				b[i][bl[i]++]=a[i][al[i]];
			}
			b[i][bl[i]]=0;
		}
		for(i=1;i<n;i++){
			if(bl[i]!=bl[i-1])goto noans;//different number of base characters
			for(j=0;j<bl[i];j++)if(b[i][j]!=b[i-1][j])goto noans;//wrong character at place j
		}
		//now we always have an answer ... find it
		goto large;
		if(n>2)goto large;
		/*
		Small only answer start;
		*/
		r=0;
		k=0;
		j=0;
		u=0;
		v=0;
		for(i=0;i<bl[0];i++){
			u=j;
			while(a[0][j++]==b[0][i]);
			v=k;
			while(a[1][k++]==b[0][i]);
			r+=abs((j-u)-(k-v));
		}
		goto ans;
		/*
		Small only answer end;
		*/
large:		/*large answer start*/
		//printf("Large answer NYI\n");
		r=0;
		for(i=0;i<bl[0];i++){
			v=0;u=1000000000;
			for(j=0;j<n;j++){
				if(i==0)cl[j]=0;
				dl[j]=cl[j];
				while(a[j][cl[j]++]==b[0][i]);
				dl[j]=cl[j]-dl[j];
				if(dl[j]<u)u=dl[j];
				if(dl[j]>v)v=dl[j];
			}
			//k,j,w
			if(u==v)continue;
			w=1000000000;
			for(;u<v;u++){
				k=0;
				for(j=0;j<n;j++)k+=abs(dl[j]-u);
				if(k<w)w=k;
			}
			r+=w;
		}
		goto ans;
		/*large answer end*/
		continue;
noans:		printf("Fegla Won\n");
		continue;
ans:		printf("%d\n",r);
	}
	return 0;
}

