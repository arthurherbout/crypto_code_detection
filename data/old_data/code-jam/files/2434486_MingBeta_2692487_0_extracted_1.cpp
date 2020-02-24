#include <iostream>

using namespace std;

int a[1000005];

int main()
{
    freopen("A-small-attempt1.in","r",stdin);
    freopen("A-small-attempt1.out","w",stdout);
    int T,cases = 0;
    cin >> T;
    while (T--)
    {
        int n, k;
        cin >> n >> k;
        for (int i = 0 ; i < k ; i++)
            cin >> a[i];
        sort(a,a+k);
        int ans = k;
        for (int i = 1 ; i <= k ; i++)
        {
            int tmp = n, now = k - i;
            for (int j = 0 ; j < i ; j++)
                if (a[j] < tmp)
                {
                    tmp += a[j];
                }
                else
                {
                    if (tmp <= 1)
                    {
                        now = k;
                        break;
                    }
                    while (tmp <= a[j])
                    {
                        tmp += tmp - 1;
                        now ++;
                    }
                    tmp += a[j];
                }
            if (now < ans)
                ans = now;
        }
        cases ++;
        cout << "Case #" << cases << ": " << ans << endl;
    }
}