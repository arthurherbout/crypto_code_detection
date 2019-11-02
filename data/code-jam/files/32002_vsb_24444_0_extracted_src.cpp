#pragma comment(linker, "/STACK:134217728")

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define sqr(a) ((a)*(a))
#define det2(a,b,c,d) ((a)*(d) - (b)*(c))


const int N = 3010;

vector<int> vv[2*N], vh[2*N];
bool a[2*N][2*N];

int main()
{
    int i, j;
    int TST, cas;
    int x, y, dx, dy;
    int n, t;
    char str[35];
    scanf( "%d", &TST );
    for ( cas = 1 ; cas <= TST ; cas++ )
    {
        x=y=N;
        dx=1;dy=0;
        for(i=0;i<2*N;i++)
        {
            vv[i].clear();
            vh[i].clear();
        }
        scanf("%d", &n);
        //for(i=0;i<n;i++)
        while(n--)
        {
            scanf("%s%d", &str, &t);
            while(t--)
            {
                for(j=0;str[j];j++)
                {
                    if(str[j]=='F')
                    {
                        if(dx==1)
                            vv[x].push_back(y);
                        if(dx==-1)
                            vv[x-1].push_back(y);
                        if(dy==1)
                            vh[y].push_back(x);
                        if(dy==-1)
                            vh[y-1].push_back(x);
                        x += dx;
                        y += dy;
                    }
                    if(str[j]=='R')
                    {
                        swap(dx,dy);
                        dy*=-1;
                    }
                    if(str[j]=='L')
                    {
                        swap(dx,dy);
                        dx*=-1;
                    }
                }
            }
        }

        memset(a,0,sizeof(a));

        for(i=0;i<2*N;i++) if ( vh[i].size() > 2 )
        {
            sort(vh[i].begin(), vh[i].end() );
            for(j=1;j+1<vh[i].size();j+=2)
                for(t=vh[i][j];t<vh[i][j+1];t++)
                    a[i][t]=1;
        }

        for(j=0;j<2*N;j++) if ( vv[j].size() > 2 )
        {
            sort(vv[j].begin(), vv[j].end() );
            for(i=1;i+1<vv[j].size();i+=2)
                for(t=vv[j][i];t<vv[j][i+1];t++)
                    a[t][j]=1;
        }

        int res=0;
        for(i=0;i<2*N;i++)
            for(j=0;j<2*N;j++)
                res+=a[i][j];
        printf( "Case #%d: %d\n", cas, res );
    }


    return 0;
}

