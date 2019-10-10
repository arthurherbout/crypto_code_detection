#include<stdio.h>
//#include<conio.h>
 long int power(int i)
{
          if(i==0)
          return 1;
          else
          return 2*power(i-1);
          }
main()
{
     
     int t,n,j,case_id;
     long int k;
      FILE *fp,*qp;
      fp=fopen("a-large.in","r");
      qp=fopen("alargesol.in","w");
      //if(fp==NULL)
      //{
                 // printf("file not found\n");getch();return 0;
                 // }
                  //else printf("found");
      
      fscanf(fp,"%d",&t);
      for(case_id=1;case_id<=t;case_id++)
      {fprintf(qp,"Case #%d: ",case_id);
                                         fscanf(fp,"%d %ld",&n,&k);
                                         
                                         
                                        // printf("power is 2 to %d and is %ld\n",n,power(n));
                                         //printf("k is %ld and k+1 modulo is %ld\n",k,(k+1)%power(n));
                                         //getch();
                                         if(k){
                                         if((k+1)%power(n))
                                          fprintf(qp,"OFF\n");
                                          else
                                          fprintf(qp,"ON\n");}
                                          else
                                          fprintf(qp,"OFF\n");
                                         }
                                         
                                         //getch();
                                         
                                         
                                         /*printf("give any number\n");
                                         scanf("%d",&n);
                                         printf("power is %d",power(n));
                                         getch();*/
                                         }
      
