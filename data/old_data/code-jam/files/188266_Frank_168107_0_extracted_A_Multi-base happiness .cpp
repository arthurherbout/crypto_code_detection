
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0
int pam[11][100000];
int sum(int i,int base);
int run();

int main() {

	freopen("A-small-attempt0.in","r",stdin);
	freopen ("A-small-attempt0.out","w",stdout);

    int i,j;
    for (i=0; i<11; i++)
        pam[i][1]=1;
    char s[100];
    gets(s);
    int c;
    sscanf(s, "%d", &c);
    for (i=0; i<c; i++)
    {
        printf("Case #%d: %d\n", i+1, run());
    }
    return 0;
}

int test(int i, int base)
{
#if DEBUG
    printf("->(%d,%d)\n",i,base);
#endif
    if (i==0) {printf("ERROR i=0!\n"); return 1;}
    if (i<100000 && pam[base][i]!=0)
        return pam[base][i];
    if (i<100000) pam[base][i]=-1;
    int x=sum(i,base);
    int m = test(x,base);
    if (i<100000) pam[base][i]=m;
    return m;
}

int sum(int i,int base){
    int x=0;
    while(i>0){
        x+=(i%base)*(i%base);
        i=i/base;
    }
    return x;
}
int run()
{
    char s[1000];
    gets(s);
    char *p;
    p = strtok(s," ");
    int n,i,j;
    int list[20];
    for (n=0;p!=NULL; p=strtok(NULL, " "), n++)
    {
        list[n] = atoi(p);
#if DEBUG
        printf("%d\n",list[n]);
#endif
    }
    for (i=2; i<1000000; i++){
        for (j=0; j<n; j++)
            if (test(i,list[j])==-1){
                break;
            }
        if (j==n)
            return i;
    }
        
    if (i==10000000)
        printf("ERROR\n ");
    
    return 0;
    
}
