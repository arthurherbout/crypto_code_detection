#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
struct Node
{
    int day;
    int s;
    int w, e;
    bool operator < (const Node that) const
    {
        return day < that.day;
    }
};
int height[4000] , base = 2000, minHeight[4000];
int main()
{
    freopen("C:\\Users\\v-y\\Downloads\\C-small-attempt1.in","r",stdin);
    freopen("C:\\Users\\v-y\\Downloads\\C-small-attempt1.out","w",stdout);
    int t, cas = 1;
    scanf("%d",&t);
    while(t--)
    {
        int att;
        scanf("%d",&att);

        int ans = 0;
        memset(height,0,sizeof(height));
        vector<Node> vec;
        while(att--)
        {
            int d,n,w,e,s,dd,dp,ds;
            e*=2;
            w*=2;
            dp*=2;
            scanf("%d %d %d %d %d %d %d %d",&d,&n,&w,&e,&s,&dd,&dp,&ds);
            for(int i = 0; i < n ; i ++)
            {

                Node next;
                next.day = i * dd + d;
                if(dp > 0)
                {
                    next.e = e + i * dp;
                    next.w = w + i * dp;
                }
                else
                {
                    next.e = e - i * dp;
                    next.w = w - i * dp;
                }
                //printf("%d %d\n", next.e ,next.w);
                next.s = s + i * ds;
                vec.push_back(next);
            }
        }
        sort(vec.begin(),vec.end());
        //printf("%d\n",vec.size() );
        for(int i = 0 ; i < vec.size() ; i ++)
        {

            memset(minHeight,0,sizeof(minHeight));
            int j = i;
            while( j < vec.size() && vec[j].day == vec[i].day)
            {
                //printf("%d\n",j);
                j ++;
            }
            bool flag = false;
            for(int k = i ; k < j ; k ++)
            {

                for(int d = vec[k].w ; d <= vec[k].e ; d++)
                {

                    if( vec[k].s > height[ d + base ] )
                    {
                        minHeight[ d + base  ] = max( minHeight[ d + base  ], vec[k].s );
                        flag = true;
                    }

                }


            }
            if(flag)
            {
                //printf("%d\n",vec[k].day);
                ans ++;
            }
            for(int k = 0 ; k < 4000 ; k ++)
            {
                height[k] = max(height[k],minHeight[k]);
            }
            i = j - 1;
        }
        printf("Case #%d: %d\n",cas ++ ,ans);
    }
    return 0;
}
