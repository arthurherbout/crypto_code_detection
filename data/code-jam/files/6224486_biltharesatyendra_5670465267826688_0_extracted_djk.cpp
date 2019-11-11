#include<string.h>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
char s[10005];
char all[1000000];
char fir[1000000],sec[1000006];
char a[9] = "eijklmno";
char b[9][9] = {"eijklmno","ilknmeoj","jolinkem","kjmlonie","lmnoeijk","meojilkn","nkemjoli","oniekjml"};
int main()
{
//printf("%d\n",strlen(a));
int i,j;
for(i=0;i<8;i++)
{
  //  printf("%s of len %d\n",b[i],strlen(b[i]));
}
map<char,int> rev;
for(i=0;i<8;i++)
rev[a[i]]=i;
int t,ca;
scanf("%d",&t);
for(ca = 1; ca<=t;ca++)
{
    ll l,x;
scanf("%lld%lld",&l,&x);
scanf("%s",&s);
char tm = 'e';
//printf("starting with %c\n",tm);
for(i=0;i<l;i++)
{
    tm = b[rev[tm]][rev[s[i]]];
   // printf("after %c it becomes %c\n",s[i],tm);
}
//printf("after all last character is %c or %d\n",tm,rev[tm]);
ll po = x%4;
//printf("not to put power %lld\n",po);
char ne = 'e';
int i;
for(i=0;i<po;i++)
ne = b[rev[ne]][rev[tm]];
tm = ne;
//printf("finally it becomes %c\n",ne);
printf("Case #%d: ",ca);
if(tm!='l')
{
    printf("NO\n");
}
else {
    //printf("may be yes\n");
    if(x < 8)
    {
        strcpy(all,s);
        for(i=0;i<x-1;i++)
        strcat(all,s);
     //   printf("we need to investigate %s\n",all);
        int v1,v2 ;
        v1 =0;
        v2 = 0;
        char s1,s2;
        s1 = s2 = 'e';
        for(i=0;all[i];i++)
        {
           // printf("after %c %c becomes ",all[i],s1);
            s1 = b[rev[s1]][rev[all[i]]];
           // printf("%c\n",s1);
            if(s1=='i'){
                v1 = 1;
                break;
            }
        }
        for(j=l*x-1;j>i;j--)
        {  // printf("after %c %c becomes ",all[i],s2);
            s2 = b[rev[all[j]]][rev[s2]];
               //  printf("%c\n",s2);
            if(s2=='k'){
                v2 = 1;
                break;
            }
        }
        //printf("we got %d and %d\n",v1,v2);
        if(v1==1 && v2==1)printf("YES\n");
        else printf("NO\n");
    }
    else {
        strcpy(fir,s);
        strcpy(sec,s);
        for(i=0;i<3;i++)
        {
            strcat(fir,s);
            strcat(sec,s);
        }
        int v1,v2;
        v1 = v2 = 0;
        char s1,s2;
        s1 = s2 = 'e';
        for(i=0;fir[i];i++){
            s1 = b[rev[s1]][rev[fir[i]]];
            if(s1=='i'){
                v1 = 1;
                break;
            }
        }
        for(i=4*l-1;i>-1;i--)
        {
            s2 = b[rev[sec[i]]][rev[s2]];
            if(s2=='k'){
                v2 = 1;
                break;
            }
        }
         if(v1==1 && v2==1)printf("YES\n");
        else printf("NO\n");
    
    }
}
}




return 0;
}
