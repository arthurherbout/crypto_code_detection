#include<stdio.h>
#include<string.h>
#include<map>
using namespace std;
int main()
{
    char a[]="ejp mysljylc kd kxveddknmc re jsicpdrysirbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcdde kr kd eoya kw aej tysr re ujdr lkgc jv";
    char b[]="our language is impossible to understandthere are twenty six factorial possibilitiesso it is okay if you want to just give up";
    map<int,int> m;
    int l;
  //  printf("len is %d %d\n",strlen(a),strlen(b));
 
    int i;
    int c[26],d[26];
    for(i=0;i<26;i++)c[i]=d[i]=0;
    for(i=0;a[i];i++){
    m[a[i]]=b[i];
    c[a[i]-'a']=1;d[b[i]-'a']=1;}
    char fir,sec;
    m['q']='z';
    m['z']='q';
  
m[32]=32;
map<int,int> ::iterator I;
    char ans[5000];
    int t;
    scanf("%d",&t);
    int ca;
  char ch;
    for(ca=1;ca<=t;ca++)
    {
                 printf("Case #%d:",ca);
                         
                        while(1)
                        {
                                scanf("%s%c",&ans,&ch);
                        for(i=0;ans[i];i++)ans[i]=m[ans[i]];
                        printf(" %s",ans);
                        if(ch=='\n')break;
                        }
                        printf("\n");
                        
   
}
return 0;
}
