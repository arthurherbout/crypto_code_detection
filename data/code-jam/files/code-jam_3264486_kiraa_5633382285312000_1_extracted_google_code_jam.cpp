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
{

    int t,no[20];char ch[20];
    sf(t)int kk=0;

    while(t--)
    { kk++;
        cin.ignore(3,'\n');
        scanf("%s",ch);
       // printf("%s",ch);
       int z=strlen(ch);
       for(int i=0;i<z;++i)
       {
         no[i]=ch[i]-'0';
       }
       if(z>1)
       {
           int ind=999,flag=0;

       for(int i=0;i<min(z-1,ind);++i)
       {// flag=0;
           if(no[i]>no[i+1])
           {
               no[i]--;
               ind=i;flag=1;
               if(i==0)break;
               i=-1;


           }


       }
       if(ind!=-1)
       for(int i=ind+1;i<z;++i)
       {
           no[i]=9;
       }


       }
    printf("Case #%d: ",kk);
      for(int i=0;i<z;++i)
       {  if(no[0]==0&&i==0)continue;
           printf("%d",no[i]);

       }
       printf("\n");


    }

}
