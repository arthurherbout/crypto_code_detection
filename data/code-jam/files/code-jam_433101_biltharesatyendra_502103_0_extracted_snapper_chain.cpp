#include<stdio.h>
#include<conio.h>
int power(int i)
{
          if(i==0)
          return 1;
          else
          return 2*power(i-1);
          }
main()
{
     
     int t,k,n,j,case_id;
      FILE *fp,*qp;
      fp=fopen("a-small-attempt1.in","r");
      qp=fopen("a-small-attempt1sol.in","w");
      //if(fp==NULL)
      //{
                 // printf("file not found\n");getch();return 0;
                 // }
                  //else printf("found");
      
      fscanf(fp,"%d",&t);
      for(case_id=1;case_id<=t;case_id++)
      {fprintf(qp,"Case #%d: ",case_id);
                                         fscanf(fp,"%d %d",&n,&k);
                                         if(k){
                                         if((k+1)%power(n))
                                          fprintf(qp,"OFF\n");
                                          else
                                          fprintf(qp,"ON\n");}
                                          else
                                          fprintf(qp,"OFF\n");
                                         }
                                         
                                         
                                         
                                         /*printf("give any number\n");
                                         scanf("%d",&n);
                                         printf("power is %d",power(n));
                                         getch();*/
                                         }
      
