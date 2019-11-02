#include<stdio.h>
//#include<conio.h>
main()
{
      int t,n,a[2],b[2],id,flag,i;
      FILE *fp,*qp;
      fp=fopen("a-small-attempt3.in","r");
      qp=fopen("a-sol.in","w");
      fscanf(fp,"%d",&t);
      printf("you gave t=%d\n",t);//getch();
      for(id=1;id<=t;id++)
      {
                          printf("Case #%d: ",id);
                          fprintf(qp,"Case #%d: ",id);
                       fscanf(fp,"%d",&n);
                       if(n==1){fscanf(fp,"%d%d",&a[0],&b[0]);
                       fprintf(qp,"0\n");printf("0\n");}
                                          else
                                               {
                                                for(i=0;i<n;i++)
                                                                fscanf(fp,"%d%d",&a[i],&b[i]);
                                                                if((a[0]-a[1])*(b[0]-b[1])<0)
                                                               { fprintf(qp,"1\n");printf("1\n");}
                                                                 else
                                                                { fprintf(qp,"0\n");printf("0\n");}
                                                                
                                                                  }
                         }
                          // getch();
                           }
