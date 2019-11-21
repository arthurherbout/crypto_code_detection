#include<stdio.h>
#include<string.h>
#include<ctype.h>
    FILE *fpr,*fpw;
int main()
{

//int L,D,N,K,i,j,p,q,m,tempm,tempj;
int T,i,j,k,ke,r,update,h,tries,sum,s;
char input[500][23];
int base[500][9];
int lim[500];
    if((fpr=fopen("A-small.in","r"))==NULL)
                                          {fprintf(stdout,"cant open input file \n");return 1;}
    if((fpw=fopen("A-small.out","w"))==NULL)
                                          {fprintf(stdout,"cant open output file \n");return 1;}
    fflush(NULL);
    
    fscanf(fpr,"%d\n",&T);
    for(i=0;i<T;i++)
    {
                    fgets(input[i],23,fpr);
    }
    for(i=0;i<T;i++)
    {
                    
                    for(j=0,k=0;j<22&&input[i][j]!='\0';j++)
                    {
                                                            
                                                        if(input[i][j]==' ')
                                                        {
                                                                          k++;
                                                        }                       
                                                        else if(input[i][j]>'1'&&input[i][j]<='9')
                                                        {
                                                             base[i][k]=input[i][j]-'0';
                                                        }                    
                                                        else if(input[i][j]=='1'&&input[i][j+1]=='0')
                                                        {
                                                             base[i][k]=10;
                                                        }
                    }
                    lim[i]=k+1;
    }
    for(i=0,ke=2;i<T;)
    {
                    if(update==1)
                    {
                    }
                    else
                    {
                        ke=2;
                    }
                    update=0;
                    for(j=lim[i]-1;j>=0;)
                    {
                                            h=0;
                                         for(r=ke,tries=1;tries<50;tries++)
                                         {
                                                sum=0;
                                                for(;r!=0;)
                                                {
                                                           s=r%base[i][j];
                                                       sum=sum+(s)*(s);
                                                       r=r/base[i][j];
                                                }
                                                if(sum==1)
                                                {
                                                          h=1;
                                                          break;
                                                }
                                                else
                                                {
                                                    r=sum;
                                                }
                                         }
                                         if(h==1)
                                         {
                                                 j--;         
                                         }
                                         else
                                         {
                                             ke++;update=1;
                                         }
                                         
                    }
                    if(update==1)
                    {
                        continue;         
                    }
                    else
                    {
                        i++;
                    }
                    fprintf(fpw,"Case #%d: %d\n",i,ke);
    }
}                  
