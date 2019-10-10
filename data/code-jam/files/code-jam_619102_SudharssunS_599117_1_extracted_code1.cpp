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
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);

    long int t;
    long int n,a[1020],b[1020];
    cin>>t;
    for(long int test=1;test<=t;test++)
    {
        cout << "Case #" << test << ": ";
        cin>>n;

        for(long int i=0;i<n;i++)
            cin>>a[i]>>b[i];
        long int count=0;
        /*if(n==1)
            cout<<"0"<<endl;
        else
        {
            if((a[0]>a[1]&&b[0]<b[1])||(a[0]<a[1]&&b[0]>b[1]))
                cout<<"1"<<endl;
            else
                cout<<"0"<<endl;
        }*/
        for(long int i=0;i<n;i++)
        {
            for(long int j=i+1;j<n;j++)
            {
                    if((a[j]>a[i]&&b[j]<b[i])||(a[j]<a[i]&&b[j]>b[i]))
                        count++;
            }
        }
        cout<<count<<endl;
    }
    return 0;
}
