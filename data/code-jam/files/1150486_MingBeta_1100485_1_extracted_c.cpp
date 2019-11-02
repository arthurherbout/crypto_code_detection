#include<iostream>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<map>
#include<cstring>

using namespace std;

long long p[1000005],ip[1000005],m;
long long n;

int main()
{
    freopen("C-large.in","r",stdin);
    freopen("C-large.out","w",stdout);
    memset(p,0,sizeof(p));
    for (int i=2;i<=1001;i++)
        if (!p[i])
           for (int j=i*2;j<1000005;j+=i) p[j]=true;
    for (int i=2;i<=1000005;i++) 
        if (!p[i]){ m++; ip[m]=i; }
    int cs;
    cin>>cs;
    for (int k=1;k<=cs;k++){
        cin>>n;
        printf("Case #%d: ",k);
        if (n<=1) { cout<<0<<endl; continue; }
        int ans=1;
        for (int i=1;i<=m;i++){
            if (ip[i]*ip[i]>n) break;
            long long t=n;
            while (t>=ip[i]){ t/=ip[i]; ans++; }
            ans--;
            }
        cout<<ans<<endl;
        }
}
