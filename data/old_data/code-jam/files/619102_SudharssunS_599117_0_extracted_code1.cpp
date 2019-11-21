#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iostream>
#include <queue>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    freopen("A-small-attempt2.in","r",stdin);
    freopen("A-small.out","w",stdout);

    int t;
    int n,a[1020],b[1020];
    cin>>t;
    for(int test=1;test<=t;test++)
    {
        cout << "Case #" << test << ": ";
        cin>>n;

        for(int i=0;i<n;i++)
            cin>>a[i]>>b[i];
        int count=0;
        if(n==1)
            cout<<"0"<<endl;
        else
        {
            if((a[0]>a[1]&&b[0]<b[1])||(a[0]<a[1]&&b[0]>b[1]))
                cout<<"1"<<endl;
            else
                cout<<"0"<<endl;
        }
        /*for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(i==j)
                    continue;
                else
                {
                    if((a[j]>a[i]&&b[j]<a[i])||(a[j]<a[i]&&b[j]>a[i]))


                        count++;
                }
            }
        }
        if(count!=1)
        count/=2;
        cout<<count<<endl;*/
    }
    return 0;
}
