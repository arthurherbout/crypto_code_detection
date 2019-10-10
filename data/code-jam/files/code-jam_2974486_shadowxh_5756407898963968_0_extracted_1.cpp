#include<stdio.h>
#include<string.h>
#include<set>
using namespace std;
set<int> s;
int main()
{
    //freopen("A-small-attempt4.in","r",stdin);
    //freopen("A-small-attempt4.out","w",stdout);
    int i,j;
    int t,cases,tmp;
    scanf("%d",&t);
    for(cases=1;cases<=t;cases++)
    {
        int r1,r2;
        int sum=0,ans=0;
        if(!s.empty()) s.clear();
        scanf("%d",&r1);
        for(i=1;i<=4;i++)
            for(j=1;j<=4;j++)
            {
                scanf("%d",&tmp);
                if(i==r1) s.insert(tmp),sum+=tmp;
            }
        scanf("%d",&r2);
        for(i=1;i<=4;i++)
            for(j=1;j<=4;j++)
            {
                scanf("%d",&tmp);
                if(i==r2) s.insert(tmp),sum+=tmp;

            }
        set<int>::iterator it;
        printf("Case #%d: ",cases);
        if(s.size()==7)
        {
            for(it=s.begin();it!=s.end();it++) ans+=*it;
            printf("%d",sum-ans);
        }
        else if(s.size()==8) printf("Volunteer cheated!");
        else printf("Bad magician!");
        printf("\n");
    }

    return 0;
}
