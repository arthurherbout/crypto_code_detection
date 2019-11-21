#include <iostream>
#include <bits/stdc++.h>

using namespace std;
long long myPow(long long x, int p){
    if(p == 0) return 1;
    long long tmp = myPow(x, p/2);
    tmp *= tmp;
    if(p%2)
        tmp *= x;
    return tmp;
}
int main()
{
    freopen("D-small-attempt1.in", "r", stdin);
    freopen("out.txt", "w", stdout);
    int t; cin >> t;
    int c = 1;
    while(t--){
        cout << "Case #" << c++ <<": ";
        long long k, c, s;
        cin >> k >> c >> s;
        if(k == 1)cout << 1 << "\n";
        else if(s < 2) cout << "IMPOSSIBLE\n";
        else{
            long long lim = myPow(k, c);
            cout << "2 " << lim-1 << "\n";
        }
    }
    return 0;
}
