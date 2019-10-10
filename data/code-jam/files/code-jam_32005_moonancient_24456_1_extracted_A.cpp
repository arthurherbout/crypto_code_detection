#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

int n,m;
int xmin,xmax;
int ymin,ymax;

vector<pair<int,int> > inside;
vector<pair<int,int> > outside;
int t;
int main()
{
    freopen("A.in","r",stdin);
    freopen("A.out","w",stdout);
    
    cin>>t;
    for (int I=1;I<=t && printf("Case #%d:\n",I);I++)
    {
        xmin=0x7FFFFFFF;
        xmax=0x80000000;
        ymin=0x7FFFFFFF;
        ymax=0x80000000;
        
        inside.clear();
        outside.clear();
        
        cin>>n;
        while (n--)
        {
            int x,y;
            char str[100];
            cin>>x>>y;
            cin.ignore(0x7FFFFFFF,' ');
            gets(str);
            if (string(str)=="BIRD")
            {
                xmin=min(xmin,x);
                xmax=max(xmax,x);
                ymin=min(ymin,y);
                ymax=max(ymax,y);
                inside.push_back(pair<int,int>(x,y));
            }
            else
            {
                outside.push_back(pair<int,int>(x,y));
            }
        }
        
        cin>>m;
        while (m--)
        {
            int x,y;
            cin>>x>>y;
            if (x<=xmax && x>=xmin && y<=ymax && y>=ymin)
            {
                puts("BIRD");
                continue;
            }

            int xmin2=min(xmin,x);
            int xmax2=max(xmax,x);
            int ymin2=min(ymin,y);
            int ymax2=max(ymax,y);
            
            bool flag=true;
            for (int i=0;i<outside.size() && flag;i++)
            {
                if (outside[i].first>=xmin2 && outside[i].first<=xmax2 && outside[i].second>=ymin2 && outside[i].second<=ymax2) flag=false;
            }
            if (flag) puts("UNKNOWN");
            else puts("NOT BIRD");
        }
    }
    return 0;
}
