#include<stdio.h>
#include <math.h>
#include <stdlib.h>
int x,i,j,T;
char a[50],b[50];
 double t,r,y,d;


int main(){
FILE * fin, *fout;
char temp[8];
fin =fopen("c:\\input.txt","r");
fout=fopen("c:\\output","w");
fscanf(fin,"%d",&j);


for (x=1;x<=j;x++){

fscanf(fin,"%s %s",&a,&b);
r=atof(a);
t=atof(b);
printf("%.0lf %.0lf\n",r,t);
//i=0;
//i= (2*r-1)*n+2*n*n;
d=(2.0*r-1.0)*(2*(r)-1.0)+8.0*(t);
//if (d>0){

	y=(-2.0*(r)+1.0+sqrt(d))/4.0;
	printf(" %d %d\n",d,y);
//}// else if (d==0){
	//y=(1.0-2.0*r)/4.0;
//} 

//	for(i=0;i<4;i++){
		
		//fgets(b[i],100,fin);
//		for(j=0;j<4;j++){
			//fscanf(fin,"%c",&b[i][j]);
		//	printf("%c",b[i][j]);
			
		//	printf("%d",a[i][j]);
//		}
		//fgets(temp,sizeof(temp),fin);
		
	//	printf("\n");
//	}


	
	fprintf(fout,"Case #%d: %.0lf",x,floor(y));
	
	fprintf(fout,"\n");
}
fclose(fin);
fclose(fout);
return 0;
}
