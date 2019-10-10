#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define pp pair<int,int>
#define mp make_pair
#define pb push_back
#define S second
#define F first

#define sf(x) scanf("%d",&x);
#define sff(x,y) scanf("%d%d",&x,&y);
 //string s1="sura";
const int MAX=2e5+100;
const int MOD=1e9+7;

int main()
{ int z=0;

 int t,k,counta=0,flag=0;
 char ch[1001];
 sf(t)
 while(t--)
 { counta=0;flag=0;z++;
   scanf("%s",ch);
   sf(k);
   int sizee=strlen(ch);
   for(int i=0;i<sizee;++i)
   {  if(ch[i]=='-')
     {
           //printf("%d ",i);

       for(int j=i;j<=i+k-1;++j)
       {
            if(j>=sizee){flag=1; break;}
           if(ch[j]=='+')
            ch[j]='-';
           else if(ch[j]=='-')
                ch[j]='+';



       }//printf("%s\n",ch);
       counta++;
     }
     if(flag==1)break;

   }
   if(flag==1)
    printf("Case #%d: IMPOSSIBLE\n",z);
    else
   {
       printf("Case #%d: %d\n",z,counta);
   }

 }

}
