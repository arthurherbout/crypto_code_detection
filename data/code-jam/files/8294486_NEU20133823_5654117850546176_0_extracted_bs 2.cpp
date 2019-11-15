#include<bits/stdc++.h>
using namespace std;
char ans[4][1005];
int cnt[4];
int main()
{
freopen("B-small-attempt0.in","r",stdin);
freopen("b-s.out","w",stdout);
    int t;
    scanf("%d",&t);
     for(int cases=1;cases<=t;cases++)
    {
        int n;
        scanf("%d",&n);
         int r,o,y,g,b,v;
        scanf("%d%d%d%d%d%d",&r,&o,&y,&g,&b,&v);
        cnt[3]=0;
        printf("Case #%d: ",cases);
        if(r>y+b||y>r+b||b>y+r)printf("IMPOSSIBLE\n");
        else
        {
            char first,last;
            if(r>=y&&r>=b)
                {
                    first='r';
                    last='r';
                    ans[3][cnt[3]++]='R';
                    r--;
                }
                else if(y>=r&&y>=b)
                {
                    first='y';
                    last='y';
                    ans[3][cnt[3]++]='Y';
                    y--;
                }
                else
                {
                    first='b';
                    last='b';
                    ans[3][cnt[3]++]='B';
                    b--;
                }
            while(r||y||b)
                {
                    char now=0;
                    int maxm=0;
                    if(last!='r'&&(now==0||r>maxm))
                    {
                        now='r';
                        maxm=r;
                    }
                    if(last!='y'&&(now==0||y>maxm))
                    {
                        now='y';
                        maxm=y;
                    }
                    if(last!='b'&&(now==0||b>maxm))
                    {
                        now='b';
                        maxm=b;
                    }
                    if(now=='r')
                    {
                        last='r';
                    ans[3][cnt[3]++]='R';
                    r--;
                    }
                    else if(now=='y')
                    {
                        last='y';
                    ans[3][cnt[3]++]='Y';
                    y--;
                    }
                    else
                    {
                        last='b';
                    ans[3][cnt[3]++]='B';
                    b--;
                    }
                }
            ans[3][cnt[3]]=0;
            if(ans[3][0]==ans[3][cnt[3]-1])
            {
                char tmp=ans[3][cnt[3]-1];
                ans[3][cnt[3]-1]=ans[3][cnt[3]-2];
                ans[3][cnt[3]-2]=tmp;
            }
            printf("%s\n",ans[3]);
        }
    }
    return 0;
}
