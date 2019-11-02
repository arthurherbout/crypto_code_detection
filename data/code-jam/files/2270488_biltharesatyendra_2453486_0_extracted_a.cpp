#include<stdio.h>
#include<vector>
#include<string.h>
#include<string>
#include<queue>
#include<algorithm>
#include<math.h>
#include<map>
#include<iostream>
#include<ctype.h>
using namespace std;
int is(char a,char b)
{
    if(a==b)return 1;
    if(a=='T')return 1;
    return 0;
}
int main()
{
    int t,ca;
    scanf("%d",&t);
    for(ca=1;ca<=t;ca++){
                         char a[5][5];
                         int i,j;
                         for(i=0;i<4;i++)scanf("%s",&a[i]);
                         int x,y;
                         x=0;y=0;
                         int count=0;
                         char ch1,ch2;
                         ch1='X'; ch2= 'O';
                         for(i=0;i<4;i++)
                         {
                              
                             if(is(a[i][0],ch1) && is(a[i][1],ch1) && is(a[i][2],ch1) && is(a[i][3],ch1))x=1;
                             if(is(a[i][0],ch2) && is(a[i][1],ch2) && is(a[i][2],ch2) && is(a[i][3],ch2))y=1;
                             
                             if(is(a[0][i],ch1) && is(a[1][i],ch1) && is(a[2][i],ch1) && is(a[3][i],ch1))x=1;
                             if(is(a[0][i],ch2) && is(a[1][i],ch2) && is(a[2][i],ch2) && is(a[3][i],ch2))y=1;
      
                                         }
     if(is(a[0][0],ch1) && is(a[1][1],ch1) && is(a[2][2],ch1) && is(a[3][3],ch1))x=1;
      if(is(a[0][0],ch2) && is(a[1][1],ch2) && is(a[2][2],ch2) && is(a[3][3],ch2))y=1;
      
       if(is(a[3][0],ch1) && is(a[2][1],ch1) && is(a[1][2],ch1) && is(a[0][3],ch1))x=1;
        if(is(a[3][0],ch2) && is(a[2][1],ch2) && is(a[1][2],ch2) && is(a[0][3],ch2))y=1;
      
      for(i=0;i<4;i++)for(j=0;j<4;j++)if(a[i][j]=='.')count++;
      printf("Case #%d: ",ca);
      if(x)printf("X won\n");
      else if(y)printf("O won\n");
      else if(count)printf("Game has not completed\n");
      else printf("Draw\n");
                                         
                         
                         }
return 0;
}
