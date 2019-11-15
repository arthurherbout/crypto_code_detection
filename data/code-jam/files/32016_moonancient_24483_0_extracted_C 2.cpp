#include <iostream>
#include <cmath>
using namespace std;

const char* ans[]=
{
"001",
"005",
"027",
"143",
"751",
"935",
"607",
"903",
"991",
"335",
"047",
"943",
"471",
"055",
"447",
"463",
"991",
"095",
"607",
"263",
"151",
"855",
"527",
"743",
"351",
"135",
"407",
"903",
"791",
"135",
"647"
};

int n;

int t;
int main()
{
freopen("C.in","r",stdin);
freopen("C.out","w",stdout);
    cin>>t;
    for (int i=1;i<=t && cout<<"Case #"<<i<<": ";i++)
    {
        cin>>n;
        puts(ans[n]);
    }
    return 0;
}
