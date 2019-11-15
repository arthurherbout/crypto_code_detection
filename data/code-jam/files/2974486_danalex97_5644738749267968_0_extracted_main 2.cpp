#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

ifstream F("p.in");
ofstream G("p.out");

const int N = 1010;

int tests,n;
vector<double> a,b;

int decieve(vector<double> a,vector<double> b)
{
    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
    int p1 = n-1;
    int p2 = n-1;
    int l = 0;
    int ans = 0;
    while ( p1 >= l )
    {
        while ( a[p1] < b[p2] && p1 >= l && p2 >= 0 )
        {
            p2--;
            l++;
        }
        if ( a[p1] > b[p2] && p1 >= l && p2 >= 0 )
        {
            ++ans;
            --p1;
            --p2;
        }
    }
    return ans;
}

int normal(vector<double> a,vector<double> b)
{
    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
    set<double> bb;

    for (int i=0;i<n;++i)
        bb.insert(b[i]);

    for (int i=0;i<n;++i)
    {
        set<double>::iterator it = bb.lower_bound(a[i]); ++it;
        if ( it == bb.end() )
        {
            int ans = (n-1) - i;
            return ans;
        }
    }
    return 0;
}

int main()
{
    F>>tests;
    for (int t=1;t<=tests;++t)
    {
        F>>n; double v;
        for (int i=0;i<n;++i) { F>>v; a.push_back(v); }
        for (int i=0;i<n;++i) { F>>v; b.push_back(v); }

        sort(a.begin(),a.end());
        sort(b.begin(),b.end());

        //for (int i=0;i<n;++i) G<<a[i]<<' '; G<<'\n';
        //for (int i=0;i<n;++i) G<<b[i]<<' '; G<<'\n';

        int y = decieve(a,b);
        int z = normal(a,b);

        G<<"Case #"<<t<<": "<<y<<' '<<z<<'\n';

        a.clear();
        b.clear();
    }
}
