#include <bits/stdc++.h>

using namespace std;

int MAX(vector<int> v)
{
    int idx = 0;
    for( int i = 1; i < v.size(); i++ )
        if( v[i] > v[idx] ) idx = i;
    return idx;
}

int f( vector<int> v )
{
    int idx = MAX(v);
    if( v[idx] <= 3 ) return v[idx];
    int r1, r2;
    vector<int> v1 = v;
    for( int i = 0; i < v1.size(); i++ )
        v1[i]--;
    r1 = 1 + f(v1);
    v.push_back(v[idx]-(v[idx]/2));
    v[idx] /= 2;
    r2 = 1 + f(v);
    return min(r1,r2);
}

int main()
{
    //freopen("B-small-attempt4.in", "r", stdin);
    //freopen("B-small-attempt4.out", "w", stdout);

    int t, d, x;
    vector<int> v;

    cin >> t;
    for( int i = 1; i <= t; i++ )
    {
        cin >> d;
        for( int j = 0; j < d; j++ )
        {
            cin >> x;
            v.push_back(x);
        }
        printf("Case #%d: %d\n", i, f(v));
        v.clear();
    }
    return 0;
}
