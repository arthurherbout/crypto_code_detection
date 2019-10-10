#include <bits/stdc++.h>

using namespace std;

int main()
{
    //freopen("A-large.in", "r", stdin);
   //freopen("A-large.out", "w", stdout);

    int t, sm, ans, sum;
    char s[1005];

    cin >> t;
    for( int i = 1; i <= t; i++ )
    {
        cin >> sm >> s;
        ans = 0;
        sum = s[0]-'0';
        if( s[0] == '0' )
        {
            ans++;
            sum = 1;
        }
        for( int j = 1; j <= sm; j++ )
        {
            if( sum < j )
            {
                ans += j-sum;
                sum += j-sum;
                sum += s[j]-'0';
            }
            else
                sum += s[j]-'0';
        }
        printf("Case #%d: %d\n", i, ans);
    }
    return 0;
}
