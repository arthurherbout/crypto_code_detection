#include <stdio.h>
#include <string.h>
int main() {
    FILE * filein, *fileout;
    int i,t,n,j,k,r,c,pos,blue;
    int done[50][50];
    char arr[50][50],outp[50][50];     
    filein=fopen("input", "r");
    fileout=fopen("output","w");
    fscanf(filein,"%d", &t);
    for(i=0;i<t;i++) {
	fscanf(filein, "%d %d", &r,&c);
	blue=0;
        for(j=0;j<r;j++) {
			fscanf(filein,"%s",arr[j]);
		//	strcpy(outp[j],"");
			for(k=0;k<c;k++) {
				done[j][k]=0;
				outp[j][k]=0;
				if(arr[j][k]==35)
					blue++;
			}
        }
	pos=1;
	if (blue%4!=0){
		pos=0;
	} else {
		for(j=0;j<r;j++) {
			for(k=0;k<c;k++) {
				if(arr[j][k]==35) {
					if(done[j][k]){
					} else {
						if(k!=c-1||j!=r-1) {
							if(done[j+1][k]||done[j+1][k+1]||done[j][k+1]) {
								pos=0;
								break;
							} else if((arr[j+1][k]!=35)||(arr[j+1][k+1]!=35)||(arr[j][k+1]!=35)){
								pos=0;
								break;
							} else {
								done[j][k]=1;
								done[j+1][k]=1;
								done[j+1][k+1]=1;
								done[j][k+1]=1;
								outp[j][k]='/';
								outp[j+1][k]='\\';
								outp[j+1][k+1]='/';
								outp[j][k+1]='\\';
															
							}
						} else {
							pos=0;
							break;

						}
					}
								
				} else if (arr[j][k]=='.') {
					outp[j][k]='.';
					done[j][k]=1;
				}
				
			}
			if(pos==0) break;
        	}
	}

        fprintf(fileout, "Case #%d:\n",i+1);
	if (pos==0){
		fprintf(fileout,"Impossible\n");
	} else {
		for(j=0;j<r;j++) {	
			for(k=0;k<c;k++){
				fprintf(fileout,"%c",outp[j][k]);
			}
			fprintf(fileout,"\n");
		}
//		fprintf(fileout, "\n");
	}
    } 
 
    fclose(filein);
    fclose(fileout);
    return 0;
}
