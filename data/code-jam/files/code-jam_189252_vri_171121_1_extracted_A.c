#include<stdio.h>
#include<string.h>
#include<ctype.h>
    FILE *fpr,*fpw;
int main()
{

//int L,D,N,K,i,j,p,q,m,tempm,tempj;
//int T,i,j,k,ke,r,update,h,tries,sum,s;
int T,i,k,j;
char input[62];
char code[62];
char order[36];
int sym[36][63];
//int base[500][9];
//int lim[500];
int nosym;
double min;
//long ans;
    if((fpr=fopen("A-large.in","r"))==NULL)
                                          {fprintf(stdout,"cant open input file \n");return 1;}
    if((fpw=fopen("A.out","w"))==NULL)
                                          {fprintf(stdout,"cant open output file \n");return 1;}
    fflush(NULL);
    
    fscanf(fpr,"%d\n",&T);
    for(i=0;i<T;i++)
    {
                    nosym=0;
                    min=0;
                    for(j=0;j<62;j++)
                    {
                                      input[j]=0;code[j]=0;
                    }
                    for(j=0;j<36;j++)
                    {
                                      order[j]=0;
                                          for(k=0;k<63;k++)
                                          {
                                                           sym[j][k]=0;
                                          }
                    }
                    fgets(input,61,fpr);
                    
                    for(j=0,k=0;j<strlen(input);j++)
                    {
                                      if(isdigit(input[j]))
                                      {
                                                           sym[input[j]-'0'][0]++;
                                                           sym[input[j]-'0'][j+1]=1;
                                                           if(sym[input[j]-'0'][0]==1)
                                                           {
                                                                      order[k++]=input[j];                
                                                           }
                                      }
                                      else if(isalpha(input[j]))
                                      {
                                          sym[input[j]-'a'+10][0]++;
                                          sym[input[j]-'a'+10][j+1]=1;
                                          if(sym[input[j]-'a'+10][0]==1)
                                          {
                                                     order[k++]=input[j];                
                                          }
                                      }                        
                                      else
                                      {
                                          
                                      }                                      
                    }
                    nosym=0;
                    for(j=0;j<36;j++)
                    {
                                     if(sym[j][0]>0)nosym++;
                    }

//                    if(nosym!=k) {printf("error1%d ",k);}
//                    printf("%d\n",nosym);
                    
                    for(j=0;j<nosym;j++)
                    {
                                        if(j==0)
                                        {
                                                if(isdigit(order[j]))
                                             {
                                                           sym[order[j]-'0'][62]='1';
                                             }
                                             else if(isalpha(order[j]))
                                             {
                                                  sym[order[j]-'a'+10][62]='1';
                                             }
                                             else
                                             {    
                                             }                                      
                                     }
                                     else if(j==1)
                                     {
                                                if(isdigit(order[j]))
                                             {
                                                           sym[order[j]-'0'][62]='0';
                                             }
                                             else if(isalpha(order[j]))
                                             {
                                                  sym[order[j]-'a'+10][62]='0';
                                             }
                                             else
                                             {    
                                             }                                      
                                         
                                     }
                                     else
                                     {
                                                if(isdigit(order[j]))
                                             {
                                                           sym[order[j]-'0'][62]='0'+j;
                                             }
                                             else if(isalpha(order[j]))
                                             {
                                                  sym[order[j]-'a'+10][62]='0'+j;
                                             }
                                             else
                                             {    
                                             }                                      
                                         
                                     }
                                      
                    }
                    for(j=0;j<strlen(input);j++)
                    {
                                                if(isdigit(input[j]))
                                             {
                                                           code[j]=sym[input[j]-'0'][62];
                                             }
                                             else if(isalpha(input[j]))
                                             {
                                                  code[j]=sym[input[j]-'a'+10][62];
                                             }
                                             else
                                             {    
                                             }                                                  
                    }
                    code[j]='\0';
                    
//                    printf("%s\n",code);
                    min=0;
                    if(nosym<2)
                    {
                               nosym=2;
                    }
                    for(j=0;j<strlen(code);j++)
                    {
//                          printf("%d %d\n",min,code[j]-'0');
                          min=min*nosym+(code[j]-'0');   
                          //printf("%d %d",min,code[j]-'0');                     
                    }
 /*                    for(j=0;j<strlen(input);j++)
                    {
                                     if(j==0)
                                     {
                                             code[j]='1';
                                             if(isdigit(input[j]))
                                             {
                                                           sym[input[j]-'0'][62]='1';
                                             }
                                             else if(isalpha(input[j])
                                             {
                                                  sym[input[j]-'a'+10][62]='1';
                                             }
                                             else
                                             {    
                                             }                                      
                                     }
                                     else if(j==1)
                                     {
                                         
                                     }
                                     
                                        
                    }
*/

//                    code[j]='\0';
                    fprintf(fpw,"Case #%d: %.0lf\n",i+1,min);
    }
}                  
