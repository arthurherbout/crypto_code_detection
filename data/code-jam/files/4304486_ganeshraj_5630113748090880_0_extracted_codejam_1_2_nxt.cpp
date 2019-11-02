#include<cstring>
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
using namespace std;

int main()
{
     freopen("binput1_2.in","r",stdin);
    freopen("cjout1.txt","w",stdout);

    int t,n,val;
    cin>>t;
    for(int tt=1;tt<=t;++tt)
    {
        cin>>n;
        int cnt[2501];
        for(int i=0;i<=2501;++i)
            cnt[i]=0;
        for(int i=1;i<2*n;++i)
        {
            for(int j=0;j<n;++j)
            {
                cin>>val;
                cnt[val]++;
            }
        }
        cout<<"Case #"<<tt<<": ";
                   for(int k=0;k<=2500;++k)
                    if(cnt[k]%2==1)
                        cout<<k<<" ";
                    cout<<endl;

    }
    return 0;
}


