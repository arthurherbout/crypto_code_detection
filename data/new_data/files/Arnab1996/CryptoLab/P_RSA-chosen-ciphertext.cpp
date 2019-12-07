#include <bits/stdc++.h>
using namespace std;
int getD(int p, int c, int n)
{
    int d = 0;
    int r = 1;
    while(1)
    {
            if((p-r)%n==0)
                    break;
            d++;
            r = r*c;
    }
    return d;
}
int main()
{
    int p = 137;
    int e = 7;
    int n = 143;
    int c = 57;
    int d = getD(p, c, n);
    cout << d << endl;
    return 0;
}