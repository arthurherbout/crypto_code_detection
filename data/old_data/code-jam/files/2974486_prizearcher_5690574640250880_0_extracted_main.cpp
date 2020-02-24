#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

FILE *f1=fopen("C-small-attempt1.in","r");
FILE *f2=fopen("C-small-attempt1.out","w");

int t,r,c,m,flag,M;
char s[61][61];

int main(int argc, char *argv[])
{
    fscanf(f1,"%d",&t);
    for(int k=1;k<=t;k++)
    {
      fscanf(f1,"%d%d%d",&r,&c,&m);flag=0;M=m;
      for(int i=1;i<=r;i++)for(int j=1;j<=c;j++)s[i][j]='.';
      for(int i=0;i<=r+1;i++)s[i][0]=s[i][c+1]='k';
      for(int i=0;i<=c+1;i++)s[0][i]=s[r+1][i]='k';
      for(int i=1;i<=r-2;i++)
      {
        for(int j=1;j<=c;j++)
          if(M)
          {
            s[i][j]='*';
            M--;
          }
          else break;
        if(!M)break;
      }
      for(int i=1;i<=c;i++)
      {
        for(int j=max(1,r-1);j<=r;j++)
          if(M)
          {
            s[j][i]='*';
            M--;
          }
          else break;
        if(!M)break;
      }
      for(int i=1;i<=r;i++)
        for(int j=1;j<=c;j++)
          if(s[i][j]=='.')
            if(s[i-1][j-1]=='*' || s[i-1][j]=='*' || s[i-1][j+1]=='*' || s[i][j-1]=='*' || s[i][j+1]=='*' || s[i+1][j-1]=='*' || s[i+1][j]=='*' || s[i+1][j+1]=='*')s[i][j]='x';
      for(int i=1;i<=r;i++)
      {
        for(int j=1;j<=c;j++)
        {
          if(s[i][j]=='x')
          {
            if(!(s[i-1][j-1]=='.' || s[i-1][j]=='.' || s[i-1][j+1]=='.' || s[i][j-1]=='.' || s[i][j+1]=='.' || s[i+1][j-1]=='.' || s[i+1][j]=='.' || s[i+1][j+1]=='.'))
            {
              flag=1;
              break;
            }
          }
        }
        if(flag)break;
      }
      fprintf(f2,"Case #%d:\n",k);
      if(flag && r*c-m!=1)fprintf(f2,"Impossible\n");
      else
      {
        s[r][c]='c';
        for(int i=1;i<=r;i++)
        {
          for(int j=1;j<=c;j++)
          {
            if(s[i][j]=='x')fprintf(f2,".");
            else fprintf(f2,"%c",s[i][j]);
          }
          fprintf(f2,"\n");
        }
      }
    }
    return 0;
}
