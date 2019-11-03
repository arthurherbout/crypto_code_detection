#include <iostream>
#include <algorithm>
using namespace std;

int n,m;
int a;

int t;

void Work(int A)
{
    for (int x1=0;x1<=n;x1++)
    for (int y1=0;y1<=m;y1++)
    for (int x2=0;x2<=n;x2++)
    for (int y2=0;y2<=m;y2++)
    {
        if (x1*y2-x2*y1==A)
        {
            cout<<0<<' '<<0<<' '<<x1<<' '<<y1<<' '<<x2<<' '<<y2<<endl;
            return;
        }
    }
    puts("IMPOSSIBLE");
    return;
}

int main()
{
freopen("B.in","r",stdin);
freopen("B.out","w",stdout);
    cin>>t;
    for (int i=1;i<=t && printf("Case #%d: ",i);i++)
    {
        cin>>n>>m>>a;
        Work(a);
    }
    return 0;
}
