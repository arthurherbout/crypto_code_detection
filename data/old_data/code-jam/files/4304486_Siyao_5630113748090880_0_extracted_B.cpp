#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;

int co[2510];

int main()
{
  int  T,N,tmp;
  scanf("%d",&T);
  for(int i=1;i<=T;++i)
  {
    printf("Case #%d:", i);
    // input
    scanf("%d",&N);
    for(int j=0;j<2510;++j) co[j]=0;
    
    for(int j=0;j<2*N-1;++j)
    {
      for(int i=0;i<N;++i)
	{
	  scanf("%d",&tmp);
	  co[tmp]=1-co[tmp];
	}
     }
     for(int j=0;j<2510;++j) 
     {
       if(co[j]==1) printf(" %d",j);
     }
     printf("\n");
  
  }
  //printf("%d\n",maxt);
  return 0;
}
