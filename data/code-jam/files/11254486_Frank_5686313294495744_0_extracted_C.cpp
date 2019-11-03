#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <bitset>
#include<string>
#include<sstream>
#include<map>
#include<algorithm>
#include<set>
using namespace std;

#include<math.h>

int n;

struct Topic
{
    string s[2];
    bool fake;
}topic[2000];

bool is_fake(int pt, int k)
{
    if (topic[pt].fake) return false;

    for (int i = 0; i < n; i++)
    {
        if (pt == i || topic[i].fake) continue;
        if (topic[pt].s[k].compare(topic[i].s[k]) == 0)
            return true;
    }
    return false;
}

void solve()
{
    
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {   
        cin >> topic[i].s[0] >> topic[i].s[1];
        topic[i].fake = false;
    }

    int ans = 0;
    for (int i = 0; i < n; i++)
    {
        if (is_fake(i, 0) && is_fake(i, 1))
        {
            ans++;
            topic[i].fake = true;
        }
    }

    printf("%d\n", ans);
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("data.txt","r",stdin);
    freopen("out.txt","w",stdout);
#endif

    int T;
    
    scanf("%d", &T);
    for(int i = 1; i <= T; i++)
    {
        printf("Case #%d: ", i);
        solve();
    }
    return 0;  
}
