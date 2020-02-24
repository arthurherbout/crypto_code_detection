#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
    FILE *fpr,*fpw;
int main()
{

//int L,D,N,K,i,j,p,q,m,tempm,tempj;
//int T,i,j,k,ke,r,update,h,tries,sum,s;
int T,N;
int i,j;
double x[500],vx[500],y[500],vy[500],z[500],vz[500];

double xm,ym,zm,xm0,ym0,zm0;double vxm,vym,vzm;
 double t,tmin;
 double dmin;
//long long d;
double d;

//char input[500][23];
//int base[500][9];
//int lim[500];
    if((fpr=fopen("B-small.in","r"))==NULL)
                                          {fprintf(stdout,"cant open input file \n");return 1;}
    if((fpw=fopen("B.out","w"))==NULL)
                                          {fprintf(stdout,"cant open output file \n");return 1;}
    fflush(NULL);
    
    fscanf(fpr,"%d\n",&T);
    for(i=0;i<T;i++)
    {
                        fscanf(fpr,"%d\n",&N);
                        vxm=0.0;
                        vym=0.0;
                        vzm=0.0;
                        xm=0.0;
                        ym=0.0;
                        zm=0.0;
                        xm0=0.0;
                        ym0=0.0;
                        zm0=0.0;
                        t=0.0;
                        tmin=0.0;
                        d=0.0;
                        dmin=0.0;
                        for(j=0;j<N;j++)
                        {
                                            fscanf(fpr,"%lf %lf %lf %lf %lf %lf\n",&x[j],&y[j],&z[j],&vx[j],&vy[j],&vz[j]);
                                            vxm+=1.0*vx[j];
                                            vym+=1.0*vy[j];
                                            vzm+=1.0*vz[j];    
                                            xm0+=1.0*x[j];
                                            ym0+=1.0*y[j];
                                            zm0+=1.0*z[j];
                                            
                        }
                        vxm=vxm/N;
                        vym=vym/N;
                        vzm=vzm/N;
                        xm0=xm0/N;
                        ym0=ym0/N;
                        zm0=zm0/N;
                        t=(-1.0)*((xm0*vxm+ym0*vym+zm0*vzm)/(vxm*vxm+vym*vym+vzm*vzm));
                        if (!(t>=0.0))
                        {
                                t=0.0;
                        }
                        else
                        {
                            
                        }
                        xm=xm0+vxm*t;
                        ym=ym0+vym*t;
                        zm=zm0+vzm*t;
                        d= xm*xm+ym*ym+zm*zm;
                        dmin=sqrt(d);
                    fprintf(fpw,"Case #%d: %lf %lf\n",i+1,dmin,t);
    }
}                  
