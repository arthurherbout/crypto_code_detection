#include<iostream>
#include<cmath>
using namespace std;

int Log2(int a)
{
    for(int i = 1; i <= 33; i++)
    {
          long long t = pow(2.0, i);
          if(t > a)
             return i-1;
    }
}

int main()
{
    freopen("B-large.in", "r", stdin);
    freopen("B-large.out", "w", stdout);
    
    int cas, icas;
    cin >> cas;
    long long L , P, C;
    for(icas = 1; icas <= cas; icas++)
    {
             cout << "Case #" << icas << ": ";
             cin >> L >> P >> C;
             int cnt = -1;
             
             while(L < P)
             {
                       cnt++;
                       L *= C;
             }
            // cout << "  " << cnt << "  ";
             if(cnt==0)
                 cout << cnt << endl;
             else
             {
                  int ans = Log2(cnt);
                  cout << ans+1 << endl;
             }
    }
    return 0;
}
