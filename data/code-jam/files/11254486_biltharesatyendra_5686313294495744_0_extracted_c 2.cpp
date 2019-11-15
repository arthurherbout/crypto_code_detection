#include<bits/stdc++.h>
using namespace std;
 string a[55],b[55];
int main()
{
    int t;
    scanf("%d",&t);
    int ca;
   
    for(ca=1;ca<=t;ca++)
    {
        int n;
        scanf("%d",&n);
        int i;
        for(i=0;i<n;i++)cin>>a[i]>>b[i];
        int ma = 1<<n;
        int mask;
        int res = 0;
        for(mask=1;mask<ma;mask++)
        {
            set<string> x,y;
            vector<int> v;
            for(i=0;i<n;i++)
            if(mask & (1<<i))
            {
                x.insert(a[i]);
                y.insert(b[i]);
            }
            else v.push_back(i);
            int vv = 1;
            for(i=0;i<v.size();i++)
            {
                int j = v[i];
                if(x.find(a[j])==x.end())
                {
                    vv=0;break;
                }
                if(y.find(b[j])==y.end())
                {
                    vv=0;break;
                }
            }
            if(vv&& v.size()>res)res = v.size();
            
        }
        printf("Case #%d: %d\n",ca,res);
    }
}
