#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

vector<int> readset()
{
    int line;
    cin>>line;
    vector<int> ret;
    for (int i=1; i<=4; i++)
        for (int j=0; j<4; j++)
        {
            int num;
            cin>>num;
            if (i==line) ret.push_back(num);
        }
    return ret;
}

int main()
{
    int tcs;
    cin>>tcs;
    for (int tc=1; tc<=tcs; tc++)
    {
        vector<int> a = readset(), b = readset();
        vector<int> ints;
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(ints));
        cout<<"Case #"<<tc<<": ";
        if (ints.size() == 0) cout<<"Volunteer cheated!\n";
        if (ints.size() > 1) cout<<"Bad magician!\n";
        if (ints.size() == 1) cout<<ints[0]<<endl;
    }
}