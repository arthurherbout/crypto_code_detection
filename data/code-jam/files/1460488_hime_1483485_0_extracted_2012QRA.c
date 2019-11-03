#include<stdio.h>
#include<string.h>
#include<ctype.h>

int main()
{
    freopen("A-small-attempt2.in","r",stdin);
    freopen("A-small-attempt2.out","w",stdout);
    char diction[27]="yhesocvxduiglbkrztnwjpfmaq",G[100];
    short T,t,n;
    scanf("%hd",&T);
    getchar();
    for(t=1;t<=T;++t)
    {
        gets(G);
        printf("Case #%hd: ",t);
        for(n=0;n<strlen(G);++n)
            if(isalpha(G[n]))
                printf("%c",diction[G[n]-'a']);
            else
                printf(" ");
        printf("\n");
    }
}
