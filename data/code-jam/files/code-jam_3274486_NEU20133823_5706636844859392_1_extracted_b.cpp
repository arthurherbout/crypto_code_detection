#include<bits/stdc++.h>
using namespace std;
struct activity
{
    int st;
    int ed;
    int cond;
};
activity act[205];
bool cmp(activity a,activity b)
{
    return a.st<b.st;
}
int store[205];
int main()
{
    freopen("B-large.in","r",stdin);
    freopen("b-b.out","w",stdout);
    int t;
    scanf("%d",&t);
    for(int cases=1;cases<=t;cases++)
    {
        int c,j;
        scanf("%d%d",&c,&j);
        //printf("%d %d\n",c,j);
        for(int i=0;i<c;i++)
        {
            scanf("%d%d",&act[i].st,&act[i].ed);
            //printf("%d %d\n",act[i].st,act[i].ed);
            act[i].cond=0;
        }
        for(int i=c;i<c+j;i++)
        {
            scanf("%d%d",&act[i].st,&act[i].ed);
            //printf("%d %d\n",act[i].st,act[i].ed);
            act[i].cond=1;
        }
        sort(act,act+c+j,cmp);
        int totc=0,totj=0;
        int ans=0;
        if(act[0].cond==0)totj=totj+act[0].st;
        else totc=totc+act[0].st;
        for(int i=0;i<c+j;i++)
        {
            if(i>0&&act[i].cond!=act[i-1].cond)ans++;
            int next;
            if(i==c+j-1)next=1440;
            else next=act[i+1].st;
            if(act[i].cond==0)totj=totj+next-act[i].st;
            else totc=totc+next-act[i].st;
        }
        if(act[c+j-1].cond!=act[0].cond)ans++;
        //printf("c:%d j:%d\n",totc,totj);
        if(totc>totj)
        {
            int cntt=0;
            int need=(totc-totj)/2;
            if(act[0].cond==1&&act[c+j-1].cond==0)
            {
                need=need-act[0].st;
                if(need<0)need=0;
            }
            else if(act[0].cond==0&&act[c+j-1].cond==1)
            {
                need=need-(1440-act[c+j-1].ed);
                if(need<0)need=0;
            }
            else if(act[0].cond==1&&act[c+j-1].cond==1)store[cntt++]=act[0].st+1440-act[c+j-1].ed;
            for(int i=1;i<c+j;i++)
            {
                if(act[i].cond==0&&act[i-1].cond==1)
                {
                    need=need-(act[i].st-act[i-1].ed);
                    if(need<=0)break;
                }
                else if(act[i].cond==1&&act[i-1].cond==1)store[cntt++]=act[i].st-act[i-1].ed;
            }
            if(need>0)
            {
                sort(store,store+cntt);
                for(int i=cntt-1;i>=0;i--)
                {
                    need=need-store[i];
                    ans=ans+2;
                    if(need<=0)break;
                }
            }
        }
        else
        {
            int cntt=0;
            int need=(totj-totc)/2;
            if(act[0].cond==0&&act[c+j-1].cond==1)
            {
                need=need-act[0].st;
                if(need<0)need=0;
            }
            else if(act[0].cond==1&&act[c+j-1].cond==0)
            {
                need=need-(1440-act[c+j-1].ed);
                if(need<0)need=0;
            }
            else if(act[0].cond==0&&act[c+j-1].cond==0)store[cntt++]=act[0].st+1440-act[c+j-1].ed;
            for(int i=1;i<c+j;i++)
            {
                if(act[i].cond==1&&act[i-1].cond==0)
                {
                    need=need-(act[i].st-act[i-1].ed);
                    if(need<=0)break;
                }
                else if(act[i].cond==0&&act[i-1].cond==0)store[cntt++]=act[i].st-act[i-1].ed;
            }
            if(need>0)
            {
                sort(store,store+cntt);
                for(int i=cntt-1;i>=0;i--)
                {
                    need=need-store[i];
                    ans=ans+2;
                    if(need<=0)break;
                }
            }
        }
        printf("Case #%d: %d\n",cases,ans);

    }
    return 0;
}
