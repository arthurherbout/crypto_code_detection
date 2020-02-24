#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;

int n = 1000;

const int learnCase = 100000;
const double step = 0.1;
const int cases = 120;

void swap(int& a, int& b)
{
    int t = a;
    a = b;
    b = t;
}

vector<int> per1(int n)
{
    vector<int> p;
    for (int i = 0; i < n; ++i)
    {
        p.push_back(i);
    }
    for (int i = 0; i < n; ++i)
    {
        int q = rand() % (n - i) + i;
        swap(p[i], p[q]);
    }
    return p;
}

vector<int> per2(int n)
{
    vector<int> p;
    for (int i = 0; i < n; ++i)
    {
        p.push_back(i);
    }
    for (int i = 0; i < n; ++i)
    {
        int q = rand() % n;
        swap(p[i], p[q]);
    }
    return p;
}

double thres = 12700.0;

double diff(vector<int> v)
{
    int sq = 0;
    for (int i = 0; i < v.size(); ++i)
    {
        sq += (v[i] - i) * (v[i] - i);
    }
    return (sqrt(sq));
}

void learn()
{
    for (int i = 0; i < learnCase; ++i)
    {
        vector<int> v;
        int r = rand();
        if (r % 2)
        {
            v = per2(n);
            double d = diff(v);
            if (d < thres) thres -= step;
        }
        else
        {
            v = per1(n);
            double d = diff(v);
            if (d > thres) thres += step;
        }
        cout << thres << endl;
    }
    cout << thres << endl;
}

int test(double th)
{
    thres = th;
    int cnt = 0;
    for (int i = 0; i < cases; ++i)
    {
        vector<int> v;
        int r = rand();
        if (r % 2)
        {
            v = per1(n);
            if (diff(v) > thres) ++cnt;
        }
        else
        {
            v = per2(n);
            if (diff(v) < thres) ++cnt;
        }
    }
    return cnt;
}

int learn2()
{
    thres = 12700.0;
    for (int i = 0; i < learnCase; ++i)
    {
        bool flag = false;
        for (int j = 0; j < 10; ++j)
        {
            int t = test(thres);
            cout << t << ' ';
            if (t >= 109) flag = true;
        }
        cout << endl;
        if (!flag) thres += step;
        cout << thres << endl;
    }
}

void solve()
{
    thres = 12721.2;
    int n;
    cin >> n;
    vector<int> v;
    for (int i = 0; i < n; ++i)
    {
        int t;
        cin >> t;
        v.push_back(t);
    }
    if (diff(v) < thres) cout << "BAD" << endl;
    else cout << "GOOD" << endl;
}

void run()
{
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i)
    {
        cout << "Case #" << i + 1 << ": ";
        solve();
    }
}

int main()
{
    srand(time(0));
    // learn();
    // learn2();
//    for (int i = 0; i < 20; ++i){
//    cout << test(12721.2) << endl;}
     run();
    return 0;
}
