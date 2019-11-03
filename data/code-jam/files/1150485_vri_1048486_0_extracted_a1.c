#include <stdio.h>
//#include <string.h>
int main() {
    FILE * filein, *fileout;
    int i,t,n,j,k;
    double rpi[100], tpl[100], wp[100], owp[100],oowp[100];
    char a[100][101];
    int b[100][100];    
  //  int w[100][100];
//printf("start");
    filein=fopen("input", "r");
    fileout=fopen("output","w");
    fscanf(filein,"%d", &t);
    for(i=0;i<t;i++) {
        for(j=0;j<100;j++) {
            tpl[j]=0;
	    wp[j]=owp[j]=oowp[j]=rpi[j]=0.0;
            for(k=0;k<100;k++) {
                b[j][k]=0;
  //              w[j][k]=0;
            }
        }
        fscanf(filein,"%d",&n);
        for(j=0;j<n;j++) {
                    
                fscanf(filein,"%s",a[j]);
        }

        for(j=0;j<n;j++) {
            for(k=0;k<n;k++) {
  //             if(j>k){
//printf("%c",a[j][k]);
	               switch (a[j][k]) {
                        
        	           case '1':    b[j][k]=2;//b[k][j]=1;
 		//			w[j][k]=1;w[k][j]=0;
        	                        tpl[j]+=1.0;
			//		tpl[k]++;
 					wp[j]+=1.0;
        	                        break;
       		           case '0':	b[j][k]=1;//b[k][j]=2;
//					w[j][k]=0;w[k][j]=1;
					tpl[j]+=1.0;
			//		tpl[k]++;
			//		wp[k]++;
                	                break;
                	   
               		}
 //		} else
   //                 continue; 
            }
//printf("\n%lf",wp[j]);
        }
	for(j=0;j<n;j++) {
		owp[j]=0.0;
                for(k=0;k<n;k++) {

//			if(b[j][k]) {
				if(b[j][k]==2){
//					if(tpl[k]>1)
						owp[j]+=(1.0*(wp[k]))/(1.0*tpl[k]-1.0);
					
				} else if (b[j][k]==1){
//					if(tpl[k]>1)
						owp[j]+=(1.0*(wp[k]-1.0))/(1.0*tpl[k]-1.0);
				}
                        }
 //		}
//		if(tpl[j]>0)
			owp[j]=(1.0*owp[j])/(tpl[j]);
//		else
//			owp[j]=0;
//printf("%lf",owp[j]);
 	}
        for(j=0;j<n;j++) {
//		if(tpl[j]>0)
			wp[j]=(1.0*wp[j])/tpl[j];
//		else
//			wp[j]=0;
//printf("%lf ",wp[j]);
                for(k=0;k<n;k++){

		    if(b[j][k]) {

	                    oowp[j]+=owp[k];
		    }
		}
		if(tpl[j]>0)
			oowp[j]=1.0*oowp[j]/tpl[j];
		else 
			oowp[j]=0;
		rpi[j]=0.25*wp[j]+0.5*owp[j]+0.25*oowp[j];		
        }

        fprintf(fileout, "Case #%d:\n",i+1);
        for(j=0;j<n;j++){
            fprintf(fileout,"%lf\n",rpi[j]);
        }
    } 
 
    fclose(filein);
    fclose(fileout);
    return 0;
}
