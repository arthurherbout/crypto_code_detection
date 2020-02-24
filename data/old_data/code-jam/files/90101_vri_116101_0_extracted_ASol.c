#include<stdio.h>
#include<string.h>
#include<ctype.h>
    FILE *fpr,*fpw;
int main()
{

int L,D,N,K,i,j,p,q,m,tempm,tempj;
char dic[16][5000];
char interpret[15][15];
char input[1000];

    if((fpr=fopen("A-small.in","r"))==NULL)
                                          {fprintf(stdout,"cant open input file \n");return 1;}
    if((fpw=fopen("A-small.out","w"))==NULL)
                                          {fprintf(stdout,"cant open output file \n");return 1;}
    fflush(NULL);
    
    fscanf(fpr,"%d %d %d\n",&L,&D,&N);

    for (i=0;i<D;i++)
    {
        

        fgets(dic[i],15,fpr);
 
    }
   
          
    for(i=0;i<N;i++)
    {

         fgets(input,1000,fpr);

         fflush(NULL);
         K=0;
         for(j=0,p=0;input[j]!='\0';j++)
         {
                                    
                                    if(input[j]=='(')
                                    {
                                                     
                                                     j++;
                                                     for(q=0;input[j]!=')';q++,j++)
                                                     {
                                                                                 interpret[p][q]=input[j];
                                                     }
                                                     interpret[p][q]='\0';

                                                     p++;
                                    }
                                    else if(isalpha(input[j]))
                                    {
                                         interpret[p][0]= input[j];
                                         interpret[p][1]='\0';

                                         p++;
                                    }
         }
         
         for(j=0,m=0;;)
         {
                         tempm=m;
                         tempj=j;
                         for(q=0;q<D;)
                         {
                                         if(interpret[j][m]=='\0')
                                         {
                                                                        if(q==D-1)
                                                                        {
                                                                                  
                                                                        break;
                                                                        }
                                                                        else
                                                                        {
                                                                            q++;m=0;
                                                                            continue;
                                                                        }
                                         }
                                         else if(interpret[j][m]==dic[q][j])
                                         {
                                                                       
                                                                       if(j==L-1){K++;m=tempm+1;j=tempj;if(q==D-1)break;else {q++;continue;}}
                                                                       else{j++;m=0;continue;}

                                         }
                                         else if(interpret[j][m]!=dic[q][j])
                                         {
                                             m++;
                                         }
                                         }
                               if(q==D-1)break;
      }               

        fprintf(fpw,"Case #%d: %d\n",i+1,K);
    }

    fclose(fpr);
    fclose(fpw);
    return 0;
}
