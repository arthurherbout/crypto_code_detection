#include <iostream>
using namespace std;
typedef unsigned long long ull;
typedef unsigned int ui;

bool getdigit(ull num, int s)
{
    return num & (((ull)1) << s);
}

ull ones(int num)
{
    return (((ull) 1) << num) - 1;
}

void getremain(int s, ull st, ull en, bool bit, ull &na, ull &nb)
{
    na = st & ones(s); nb = en & ones(s);
    if (bit && !getdigit(st, s)) na = 0;
    if (!bit && getdigit(en, s)) nb = ones(s);
}

ull dfs(int s, ull a1, ull a2, ull b1, ull b2, ull k1, ull k2)
{
    if (s < 0) return 1;
    if (k1 == 0 && k2 == ones(s+1)) return (a2-a1+1)*(b2-b1+1);
    bool a1b = getdigit(a1, s), a2b = getdigit(a2, s);
    bool b1b = getdigit(b1, s), b2b = getdigit(b2, s);
    bool k1b = getdigit(k1, s), k2b = getdigit(k2, s);
    ull ret = 0;
    for (int a = a1b; a <= a2b; a++)
        for (int b = b1b; b <= b2b; b++)
            if ((a && b) == k1b || (a && b) == k2b)
            {
                ull na1, na2, nb1, nb2, nk1, nk2;
                getremain(s, a1, a2, a, na1, na2);
                getremain(s, b1, b2, b, nb1, nb2);
                getremain(s, k1, k2, a && b, nk1, nk2);
                ret += dfs(s - 1, na1, na2, nb1, nb2, nk1, nk2);
            }
    return ret;
}

int main()
{
    int t;
    cin>>t;
    for (int i=1; i<=t; i++)
    {
        ull a, b, k;
        cin>>a>>b>>k;
        cout<<"Case #"<<i<<": "<<dfs(31, 0, a-1, 0, b-1, 0, k-1)<<"\n";
    }
}