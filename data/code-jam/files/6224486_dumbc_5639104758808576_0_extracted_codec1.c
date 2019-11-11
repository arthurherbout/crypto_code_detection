#include<stdio.h>
int main()
{
    int num,loop,i,j;
    long long int fact;
    scanf("%d",&loop);
  for(i=0;i<loop;i++)
  {
    scanf("%d",&num);
     j=1;fact=1;
    while(j!=num+1)
    {
     fact=j*fact;
     j++;            
    }
    printf("%Ld\n",fact);
  }
  getch();
    return 0;
}
