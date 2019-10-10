#include<stdio.h>
#include<string.h>
#include<set>
using namespace std;
set<int> hashtab;
int n;
void add(int tmp)
{
    while(tmp)
    {
        hashtab.insert(tmp%10);
        tmp/=10;
    }
    return;
}
int main()
{
    //freopen("A-large.in","r",stdin);
    //freopen("A-large.out","w",stdout);
    int i,j;
    int cases=0;
    scanf("%d",&n);
    while(n--)
    {
        int tmp;
        scanf("%d",&tmp);
        if(!tmp) {printf("Case #%d: INSOMNIA\n",++cases);continue;}
        if(!hashtab.empty()) hashtab.clear();
        int k=0;
        while(hashtab.size()<10)
        {
            k++;
            add(tmp*k);
        }
        printf("Case #%d: %d\n",++cases,tmp*k);

    }

    return 0;
}
