#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

char a[3][100]=
{
"ejp mysljylc kd kxveddknmc re jsicpdrysi",
"rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd",
"de kr kd eoya kw aej tysr re ujdr lkgc jv"
};

char b[3][100]=
{
"our language is impossible to understand",
"there are twenty six factorial possibilities",
"so it is okay if you want to just give up"
};

char s[100000],t[100000];
char map[255];

int main(void)
{
    int i,k,n,T;
    map[(int)'q']='z';
    map[(int)'z']='q';
    for(i=0;i<3;++i)
    {
        strcpy(s,a[i]);
        strcpy(t,b[i]);
        n=strlen(s);
        for(k=0;k<n;++k)
            if (s[k]!=' ')
                map[(int)s[k]]=t[k];
    }
    /*
    for(i='a';i<='z';++i)
        cout<<map[i];
    cout<<endl;
    system("PAUSE");
    */
    freopen("A-small-attempt2.in","r",stdin);
    freopen("A-small-attempt2.out","w",stdout);
    scanf("%d",&T);
    gets(s);
    for(i=1;i<=T;++i)
    {
        gets(s);
        n=strlen(s);
        for(k=0;k<n;++k)
            if (s[k]==' ')
                t[k]=' ';
            else t[k]=map[(int)s[k]];
        t[n]='\0';
        printf("Case #%d: %s\n",i,t);
    }
    return 0;
}
