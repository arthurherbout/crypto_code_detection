#define _CRT_SECURE_NO_WARNINGS

#include <time.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

long long reverse(long long x)
{
    long long ans = 0;
    for (; x; x /= 10)
    {
        ans = ans * 10 + x % 10;
    }
    return ans;
}

long long slow_solve_(vector<long long> &d, long long n)
{
    if (d[n] > 0) return d[n];

    d[n] = 1 + slow_solve_(d, n - 1);
    long long rev = reverse(n);
    if (rev >= 1 && rev <= d.size() - 1)
    {
        d[n] = min(d[n], 1 + slow_solve_(d, rev));
    }
    return d[n];
}

long long slow_solve(long long n)
{
    vector<long long> d(n + 1, n);
    d[1] = 1;
    for (int i = 1; i < n; ++i)
    {
        d[i + 1] = min(d[i+1], d[i] + 1);
        long long rev = reverse(i);
        if (rev >= 1 && rev <= n)
        {
            /*if (d[rev] < 0)
            {
                cout << "WA!!! " << i << endl;
                return -1;
            }*/
            d[rev] = min(d[rev], d[i] + 1);
        }
    }
    return d[n];
}

long long solve(long long n)
{
    return 0;
    /*vector<long long> d(n + 1, -1);
    d[1] = 1;
    return slow_solve(d, n);*/
}

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    double total_time = clock();
    int T;
    cin >> T;
    for (int test = 0; test < T; ++test)
    {
        long long n;
        cin >> n;

        double time = clock();
        cout << "Case #" << test + 1 << ": " << slow_solve(n) << "\n";
        cerr << "Time = " << (clock() - time) / CLOCKS_PER_SEC << "sec" << endl;
    }
    cerr << "Total time = " << (clock() - total_time) / CLOCKS_PER_SEC << "sec" << endl;

    return 0;
}

