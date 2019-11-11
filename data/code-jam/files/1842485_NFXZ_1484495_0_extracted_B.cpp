#include <stdio.h>
#include <iostream>
#include <algorithm>
using namespace std;

struct Person
{
	double x;
	double y;
	double r;
}all[1005];

int main()
{
	FILE* rfp;
	FILE* wfp;
	
	rfp=fopen("B-small-attempt8.in","r");
	wfp=fopen("B-small-attempt8.out","w");
	
	long long  t,i,j,k,l;
	long long N,L,W,p[1005],tmp,flag;
	double x,y,nyr,nxr;
	
	fscanf(rfp,"%lld",&t);
	for(i=1;i<=t;i++){
		//printf("s\n");
		fprintf(wfp,"Case #%lld:",i);
		fscanf(rfp,"%lld %lld %lld",&N,&W,&L);
		for(j=0;j<N;j++){
			p[j]=j;
			fscanf(rfp,"%lf",&all[j].r);
			printf("%lf\n",all[j].r);
			all[j].r+=2;
		}
		
	
		for(j=0;j<N-1;j++){
			for(k=0;k<N-j-2;k++){
				//for(l=0;l<N;l++){
				//	printf("%d ",p[l]);
				//}
				//printf("\n");
				if(all[p[k]].r<all[p[k+1]].r){
					//printf("%d %d %lf %lf\n",p[k],p[k+1],all[p[k]].r,all[p[k+1]].r);
					tmp=p[k];
					p[k]=p[k+1];
					p[k+1]=tmp;
				}
			}
		}
		
		//printf("ok\n");

		
		flag=0;y=-all[p[0]].r;nyr=0;nxr=0;
		for(j=0;j<N;j++){
		    if(flag==0){
    			x=0;y=y+nyr+all[p[j]].r;
    			nyr=all[p[j]].r;nxr=all[p[j]].r;
    			all[p[j]].x=0;
    			all[p[j]].y=y;
    			flag=1;
    			if(y>L)printf("over\n");
    			//printf("head : %d\n",p[j]);
    		}
    		else{
    			//printf("%lf %lf %lf\n",x,nxr,all[p[j]].r);
		    	if(x+nxr+all[p[j]].r>W-1){
    		        //printf("end : %d\n",p[j]);
	    			flag=0;
	    			j--;
	    		}
	    		else{
   			        //printf("mid : %d\n",p[j]);
		    		all[p[j]].x=x+nxr+all[p[j]].r;
		    		x=all[p[j]].x;
		    		//printf("j:%d %lf %lf %lf\n",j,x,all[p[j]].x,all[p[j]].y);
		    		all[p[j]].y=y;
		    		nxr=all[p[j]].r;
		    	}
		    }
		    //if(all[p[j]].x==0&&all[p[j]].y==0)printf("%d/////\n",p[j]);
		}
		
		for(j=0;j<N;j++){
			fprintf(wfp," %.1lf %.1lf",all[j].x,all[j].y);
			//if(i<=3)
			//printf(" %.1lf %.1lf\n",all[j].x,all[j].y);
		}
		fprintf(wfp,"\n");
	//	printf("\n");
	}
	
	fclose(rfp);
	fclose(wfp);
	return 0;
}