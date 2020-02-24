#include <cstring>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <cassert>
using namespace std;

#define foreach(i, c) for (__typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)

int n;
int X, Y;

int total;
int count;

typedef vector<pair<int, int> > vii;


int is_there(const vii &c, int x, int y)
{
    if (y < 0)
        return 0;

    foreach(iter, c)
    {
        if (iter->first == x && iter->second == y)
            return 1;
    }
    return 0;
}

int y_high(const vii &c)
{
    int ret = 0;
    foreach(iter, c)
    {
        if (iter->first == 0 && iter->second > ret)
        {
            ret = iter->second;
        }
    }

    return ret;
}

void fill_all(vii c, int x, int y)
{
    if (c.size() == n)
    {
        //terminate
        // foreach(iter, c)
        // {
        //     printf("(%d %d)\t", iter->first, iter->second);
        // }
        // printf("\n");

        total ++;
        if (is_there(c, X, Y))
            count ++;

        return;
    }

    while (!is_there(c, x-1, y-1) && !is_there(c, x+1, y-1) && !is_there(c, x, y-2) && y>0)
        y-=2;

    if (y == 0)
    {
        c.push_back(make_pair(x, y));
        fill_all(c, 0, y_high(c) + 2);
    }
    else
    {    

        if (is_there(c, x-1, y-1))
        {
            if (!is_there(c, x+1, y-1))
                fill_all(c, x+1, y-1);            
        }
        else if (is_there(c, x+1, y-1))
        {            
            fill_all(c, x-1, y-1);
        }
        else if (is_there(c, x, y-2))
        {            
            fill_all(c, x-1, y-1);            
            
            fill_all(c, x+1, y-1);
        }

        if (is_there(c, x-1, y-1) && is_there(c, x+1, y-1))
        {
            c.push_back(make_pair(x,y));
            fill_all(c, 0, y_high(c) + 2);
        }
    }
}

void work()
{
    scanf("%d%d%d", &n, &X, &Y);
    vii c;
    c.push_back(make_pair(0, 0));
    total = 0;
    count = 0;

    fill_all(c, 0, 2);
    // printf("%d\t%d\t\n", count, total);
    printf("%.9lf\n", (double)count / (double) total);
}
 
int main()
{
    // freopen("A-small-practice.in", "r", stdin);
    // freopen("A-large-practice.in", "r", stdin);    
    freopen("b1.in", "r", stdin);
    freopen("out.txt", "w", stdout);
 
    int t;
    scanf("%d", &t);
    for (int cs = 1; cs <= t; cs++)
    {
        printf("Case #%d: ", cs);
        work();
    }
 
    return 0;
}