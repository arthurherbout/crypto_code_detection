#include<stdio.h>
//#include<conio.h>
main()
{
      int t,n,a[1000],b[1000],id,flag,i,j;
      long int sum;
      FILE *fp,*qp;
      fp=fopen("a-large.in","r");
      qp=fopen("a-sol.in","w");
      fscanf(fp,"%d",&t);
      printf("you gave t=%d\n",t);//getch();
      for(id=1;id<=t;id++)
      {sum=0;
                          fprintf(qp,"Case #%d: ",id);
                          fscanf(fp,"%d",&n);
                          if(n==1)
                          fscanf(fp,"%d%d",&a[0],&b[0]);
                          else
                          {
                              for(i=0;i<n;i++)
                              fscanf(fp,"%d%d",&a[i],&b[i]);
                              for(i=0;i<n-1;i++)
                              {
                                              for(j=1;j<n;j++)
                                              {
                                                              if((a[i]-a[j])*(b[i]-b[j])<0)
                                                             sum++;//printf("multiplication is %d
                                                              }
                                                              }
                                                              }
                                                              fprintf(qp,"%ld\n",sum);
                                                              }
                                                             // getch();
                                                              }
                          
