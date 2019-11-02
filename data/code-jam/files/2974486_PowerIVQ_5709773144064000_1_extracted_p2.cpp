#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
    int tcs;
    cin>>tcs;
    for (int tc=1; tc<=tcs; tc++)
    {
        double c, f, x;
        cin>>c>>f>>x;
        cout<<"Case #"<<tc<<": ";
        double ans=x/2+1;
        int farm=0;
        for (double time=0; ;)
        {
            double cost=time+x/(2+f*farm);
            if (cost>ans) break;
            ans=cost;
            time+=c/(2+f*(farm++));
        }
        printf("%.9lf\n", ans);
    }
}