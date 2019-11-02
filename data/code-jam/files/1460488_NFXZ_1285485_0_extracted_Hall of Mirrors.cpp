#include <stdio.h>
#include <iostream>
#include <set>
#include <math.h>
using namespace std;

int mmap[40][40];

int dcmp(double a){
	if(a>1e-8)return 1;
	if(a<-1e-8)return -1;
	return 0;
}

int main()
{
	int t,H,W,D,j,k,mph,mpw,i,cnt;
	double x,y,a,ta;
	char c;
	FILE* in;
	FILE* out;
	char temp[50];
	
	
	in=fopen("D-small-attempt0.in","r");
	out=fopen("D-small-attempt0.out","w");
	
	fscanf(in,"%d",&t);
	
	for(i=1;i<=t;i++)
	{
		
		fprintf(out,"Case #%d: ",i);
		
		fscanf(in,"%d %d %d",&H,&W,&D);
		
		set<double> ang;
		 
	 	cnt=0;
	 
		for(j=0;j<H;j++){
			for(k=0;k<W;k++){
				c='\n';
				while(c=='\n')fscanf(in,"%c",&c);
				if(c=='#'){
					mmap[j][k]=1;
				}
				else if(c=='.'){
					mmap[j][k]=0;
				}
				else{
					mmap[j][k]=-1;
					mph=j;mpw=k;
				}
			}
		}
		
		
		for(j=-50;j<=50;j++){
			for(k=-50;k<=50;k++){
				if(j==0&&k==0)continue;
				
				if(j==0)x=0;
				else if(j<0){
					x=-(mpw-0.5);
					x-=(abs(j)-1)*(W-2);
					if(abs(j)%2==1)
						x-=mpw-0.5;
					else
						x-=(W-2)-(mpw-0.5);
				}
				else{
					x=(W-2)-(mpw-0.5);
					x+=(abs(j)-1)*(W-2);
					if(abs(j)%2==1){
						x+=(W-2)-(mpw-0.5);
					}
					else{
						x+=mpw-0.5;
					}
				}
				/////////////////////////
				if(k==0)y=0;
				else if(k<0){
					y=-(mph-0.5);
					y-=(abs(k)-1)*(H-2);
					if(abs(k)%2==1)
						y-=mph-0.5;
					else
						y-=(H-2)-(mph-0.5);
				}
				else{
					y=(H-2)-(mph-0.5);
					y+=(abs(k)-1)*(H-2);
					if(abs(k)%2==1){
						y+=(H-2)-(mph-0.5);
					}
					else{
						y+=mph-0.5;
					}
				}

				if(dcmp(x*x+y*y-D*D)>0)continue;
				
				//printf("%d %lf %lf\n",i,x,y);
				a=atan2(y,x);
				sprintf(temp,"%.7lf",a);
				sscanf(temp,"%lf",&a);
				
				if(ang.count(a)==0){
					cnt++;
					ang.insert(a);
				}
			}
		}
		
		fprintf(out,"%d\n",cnt);

	}
	
	fclose(in);
	fclose(out);
	return 0;
}

