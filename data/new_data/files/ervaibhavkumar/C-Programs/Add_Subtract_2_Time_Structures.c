#include <stdio.h>

typedef struct
{
    int hr;
    int mm;
    int ss;
} time;

time addTime(time *t1 , time *t2)
{
    time add;
    add.hr = t1->hr + t2->hr;
    add.mm = t1->mm + t2->mm;
    add.ss = t1->ss + t2->ss;
    while(add.ss >= 60)
    {
        add.mm++;
        add.ss -= 60;
    }
    while(add.mm >= 60)
    {
        add.hr++;
        add.mm -= 60;
    }
    return add;
}

time subtractTime(time *t1 , time *t2)
{
    time sub;
    if(t1->hr > t2->hr)
    {
        sub.hr = t1->hr - t2->hr;
        sub.mm = t1->mm - t2->mm;
        sub.ss = t1->ss - t2->ss;
        while(sub.ss<0)
        {
            sub.mm--;
            sub.ss += 60;
        }
        while(sub.mm<0)
        {
            sub.hr--;
            sub.mm += 60;
        }
    }
    else
    {
        sub.hr = t2->hr - t1->hr;
        sub.mm = t2->mm - t1->mm;
        sub.ss = t2->ss - t1->ss;
        while(sub.ss<0)
        {
            sub.mm--;
            sub.ss += 60;
        }
        while(sub.mm<0)
        {
            sub.hr--;
            sub.mm += 60;
        }
    }

    return sub;
}
int main()
{
    time t1 , t2;
    printf("Enter hours for time 1 :");
    scanf("%d",&t1.hr);
    printf("Enter minutes for time 1 : ");
    scanf("%d",&t1.mm);
    printf("Enter seconds for distance 1 :");
    scanf("%d",&t1.ss);
    printf("Enter hours for time 2 :");
    scanf("%d",&t2.hr);
    printf("Enter minutes for time 2 : ");
    scanf("%d",&t2.mm);
    printf("Enter seconds for distance 2 :");
    scanf("%d",&t2.ss);
    time add = addTime(&t1,&t2);
    printf("Time after adding is %d hours %d minutes %d seconds\n",add.hr,add.mm,add.ss);
    time sub = subtractTime(&t1,&t2);
    printf("Time after subtracting is %d hours %d minutes %d seconds\n",sub.hr,sub.mm,sub.ss);
    return 0;
}
