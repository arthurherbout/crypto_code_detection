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
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small-attempt0.out","w",stdout);
    short t,T;
    scanf("%hd",&T);
    for(t=1;t<=T;++t)
    {
        short A,B,n,m,j,ans,add;
        scanf("%hd %hd",&A,&B);
        char **nums,*num;
        nums=(char **)malloc((B+1)*sizeof(char *));
        for(n=1;n<=B;++n)
        {
            num=(char *)malloc(5*sizeof(char));
            sprintf(num,"%hd",n);
            strncpy(num+strlen(num),num,strlen(num)-1);
            nums[n]=num;
        }/*
        for(n=0;n<=B-A;++n)
            for(m=n;m<=B-A;++m)
                if(strcmp(nums[n],nums[m])>0)
                {
                    char *tmp=(char *)malloc(3*sizeof(char));
                    strcpy(tmp,nums[m]);
                    strcpy(nums[m],nums[n]);
                    strcpy(nums[n],tmp);
                }
        ans=0;
        j=0;
        for(n=1;n<=B-A;++n)
        {
            if(!strcmp(nums[n],nums[n-1]))
                if(j)
                    continue;
                else
                {
                    ans+=strlen(nums[n])-1;
                    j=1;
                }
            else
                j=0;
        }*/
        ans=0;
        for(n=A;n<=B;++n)
        {
            char tmp[5];
            sprintf(tmp,"%hd",n);
            add=0;
            if(n==1000)
                break;
            switch(strlen(tmp))
            {
                case 2:
                    for(m=A;m<=99;++m)
                    {
                        if(m>B)
                            break;
                        if(strstr(nums[m],tmp))
                            ++add;
                    }
                    break;
                case 3:
                    for(m=A;m<=999;++m)
                    {
                        if(m>B)
                            break;
                        if(strstr(nums[m],tmp))
                            ++add;
                    }
                    break;
            }
            if(strlen(tmp)-1)
                --add;
            ans+=add;
//            printf("%hdadd=%hd\n",n,add);
        }
/*        for(n=0;n<=B;++n)
            printf("%s n=%hd\n",nums[n],n);*/
        printf("Case #%hd: %hd\n",t,ans/2);
    }
    return 0;
}
