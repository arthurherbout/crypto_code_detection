#include <bits/stdc++.h>

#define mod 1000000007
#define inf 1000000000000
#define root2 1.41421
#define root3 1.73205
#define pi 3.14159
#define MAX 100001
#define ll long long int
#define PII pair<int, int>
#define f first
#define s second
#define mk make_pair
#define PLL pair<ll, ll>
#define gc getchar
#define pb push_back

using namespace std;
int cnt[2505], lost[55];
int main()
{
    ifstream in("B-large(1).in");
    ofstream out("output.txt");
    int t, n, z=1, i, j, x;
    in>>t;
    while(z<=t)
    {
        out<<"Case #"<<z<<":";
        in>>n;
        memset(cnt, 0, sizeof cnt);
        for(i=0;i<(2*n-1);i++)
        {
            for(j=0;j<n;j++)
            {
                in>>x;
                cnt[x]++;
            }
        }
        x=0;
        for(i=0;i<2505;i++)
        if(cnt[i]&1)
        {
            out<<" "<<i;
            x++;
            if(x==n)
                break;
        }
        out<<endl;
        z++;
    }
    return 0;
}

