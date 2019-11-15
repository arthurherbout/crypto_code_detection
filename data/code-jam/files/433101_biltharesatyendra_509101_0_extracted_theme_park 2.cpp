#include<stdio.h>
//#include<conio.h>
main()
{
      FILE *fp,*qp;
      int sum,t,case_id,k,r,n,a[15],i,j,money,remain;
      fp=fopen("c-small-attempt1.in","r");
      qp=fopen("c-small-attempt1sol.txt","w");
      //if(fp==NULL)
      //printf("file not found");
      //else
      //printf("found");
      //getch();
      
      fscanf(fp,"%d",&t);
      for(case_id=1;case_id<=t;case_id++)
      {
                                         money=0;
                                         
                                         
         fscanf(fp,"%d%d%d",&r,&k,&n);
         //printf("you have given r=%d k=%d n=%d\n",r,k,n);
         //printf("Elements read are \n");
         for(i=0;i<n;i++)
         {fscanf(fp,"%d",&a[i]);
        // printf("%d  ",a[i]);
        }
         //printf("%\n");
         j=0;
         for(i=0;i<r;i++)
         {  sum=0;remain=n;
         //printf("\nin %d trip elements are \n",i+1);
                        while(((sum+a[j])<=k)&&remain)
                        {
                                           sum=sum+a[j];
                        //printf(" a[%d] =%d ",j,a[j]);
                        j=(j+1)%(n);remain--;
                        } 
                        
                        money=money+sum;
                        }
                        fprintf(qp,"Case #%d: %d\n",case_id,money); 
                        //printf("\nCase #%d:  %d\n",case_id,money);
                        //getch();
                        //clrscr();     
                                         
                                         
                                         }
      
      
      
      
      
      
      
      
      
      
      
      
      
      fclose(fp);
      fclose(qp);
      //getch();
      }
