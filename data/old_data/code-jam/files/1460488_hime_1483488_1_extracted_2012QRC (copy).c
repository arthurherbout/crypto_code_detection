#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int cmp_string(const void * _a,const void * _b)
{
    char *a=(char *)_a;
    char *b=(char *)_b;
    return strcmp(a,b);
}

int main()
{
    freopen("C-large.in","r",stdin);
    freopen("C-large.out","w",stdout);
    
    short t,T;
    scanf("%hd",&T);
    for(t=1;t<=T;++t)
    {
        int A,B,n,m,j,ans,add;
        scanf("%d %d",&A,&B);
        char **nums,*num;
        nums=(char **)malloc((B+1)*sizeof(char *));
        for(n=1;n<=B;++n)
        {
            num=(char *)malloc(5*sizeof(char));
            sprintf(num,"%d",n);
            strncpy(num+strlen(num),num,strlen(num)-1);
            nums[n]=num;
        }        ans=0;
        for(n=A;n<=B;++n)
        {
            char tmp[7],*sushi;
            sprintf(tmp,"%d",n);
            add=0;
            m=strlen(tmp);
            sushi=(char *)malloc(m*sizeof(char));
            for(j=0;j<m;++j)
            {
                strncpy(sushi,nums[n]+j,m);
//                printf("sushi=%s\n",sushi,m);
                if(atoi(sushi)>=A && atoi(sushi)<=B && atoi(sushi)!=n)
                    ++add;
            }
            ans+=add;
//            printf("%dadd=%d\n",n,add);
        }
/*        for(n=0;n<=B;++n)
            printf("%s n=%d\n",nums[n],n);*/
        printf("Case #%d: %d\n",t,ans/2);
    }
    return 0;
}
