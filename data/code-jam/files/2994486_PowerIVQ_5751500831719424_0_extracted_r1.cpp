#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int solve(vector<string> &ipt, int sz)
{
    vector<int> pt(sz);
    int ret = 0;
    while (true)
    {
        if (ipt[0].length() == pt[0])
        {
            for (int i=1; i<sz; i++)
                if (ipt[i].length() != pt[i]) return -1;
            return ret;
        }
        char now = ipt[0][pt[0]];
        vector<int> lens(sz);
        for (int i=0; i<sz; i++)
        {
            
            if (ipt[i][pt[i]] != now) return -1;
            while (pt[i]<ipt[i].length() && ipt[i][pt[i]] == now)
            {
                pt[i]++;
                lens[i]++;
            }
            //cout<<i<<" "<<now<<" "<<pt[i]<<" "<<lens[i]<<endl;
        }
        sort(lens.begin(), lens.end());
        for (int i=0; i<sz; i++) ret += abs(lens[i] - lens[sz/2]);
    }
}

int main()
{
    int t;
    cin>>t;
    for (int i=1; i<=t; i++)
    {
        int size;
        cin>>size;
        vector<string> ipt(size);
        for (int j=0; j<size; j++) cin>>ipt[j];
        cout<<"Case #"<<i<<": ";
        int ans = solve(ipt, size);
        if (ans==-1) cout<<"Fegla Won\n"; else cout<<ans<<"\n";
    }
}