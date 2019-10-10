#include<iostream>
#include<cstdio>
#include<memory.h>
using namespace std;

char p[3][256];
char q[3][256];
char s[256];
char a[256];
char str[256];
int main()
{
	int n;
	//scanf("%d",&n);
	//gets(str);
	freopen("A-small-attempt1.in","r",stdin);
	freopen("out.out","w",stdout);
	strcpy(p[0],"ejp mysljylc kd kxveddknmc re jsicpdrysi");
	strcpy(p[1],"rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd");
	strcpy(p[2],"de kr kd eoya kw aej tysr re ujdr lkgc jv");
	strcpy(q[0],"our language is impossible to understand");
	strcpy(q[1],"there are twenty six factorial possibilities");
 	strcpy(q[2],"so it is okay if you want to just give up");
	s['z'] = 'q';
	s['q'] = 'z';
	int l;
	for(int i=0;i<3;i++)
	{
		l = strlen(p[i]);
		for(int j=0;j<l;j++)
			s[p[i][j]] = q[i][j];
	}
	scanf("%d",&n);
	gets(str);
	for(int i=1;i<=n;i++)
	{
	    gets(str);
        l=strlen(str);
        printf("Case #%d: ",i);
        for(int i=0;i<l;i++)
            printf("%c",s[str[i]]);
        printf("\n");
	}
//	for()

}
