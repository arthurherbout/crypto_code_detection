#include<stdio.h>
//#include<conio.h>
int root(int t)
{printf("t is %d\n",t);//getch();
    int i;
    for(i=1;i*i<=t;i++);
    if((i-1)*(i-1)==t)
    return i;
    else
    return i-1;
}
int power(float ratio,int c)
{
    int i=0;
    if(ratio==c)
    return 0;
    else
    while(ratio>1)
    {
                  ratio=ratio/c;
                  i++;
                  }if(ratio==1)
                  return i;
                  else return i+1;
                  }
    
main()
{
      int t,l,c,p,id,y;
      float ratio;
      FILE *fp,*qp;
      fp=fopen("b-small-attempt1.in","r");
      qp=fopen("bsol.in","w");
      fscanf(fp,"%d",&t);
     // printf("test cases are %d",t);getch();
      for(id=1;id<=t;id++)
      {
                          fscanf(fp,"%d%d%d",&l,&p,&c);
                          ratio=float(p)/float(l);
                          
                         // printf("ratio is %f\n",ratio);getch();
                         // printf("power is %d",power(ratio,c));
                          y=power(ratio,c*c);
                          //getch();
                         // printf("root is %d\n",root(y));
                         // getch();
                          fprintf(qp,"Case #%d: %d\n",id,y);
                          }
                         // getch();
                          }
                          
      
