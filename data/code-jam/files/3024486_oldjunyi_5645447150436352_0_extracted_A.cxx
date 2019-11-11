#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

long long a[1000005];
long long s[1000005];

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        int n,P,Q,R,S;
        scanf("%d%d%d%d%d",&n,&P,&Q,&R,&S);
        for(int i=0;i<n;i++){
            a[i]=(i*1ll*P+Q)%R+S;
            s[i+1]=s[i]+a[i];
        }
        long long ans=0;
        for(int i=0;i<n;i++){
            int lo=i,hi=n;
            while(lo<hi-5){
                int l=(lo*5+hi*3)/8;
                int r=(lo*3+hi*5)/8;
                long long p=max(s[l]-s[i],s[n]-s[l]);
                long long q=max(s[r]-s[i],s[n]-s[r]);
                if(p<q) hi=r; else lo=l;
            }
            long long use=1ll<<60;
            for(int x=lo;x<=hi;x++)
                use=min(use,max(s[x]-s[i],s[n]-s[x]));
            ans=max(s[n]-max(s[i],use),ans);
        }
        printf("Case #%d: %.15f\n",++no,ans*1.0/s[n]);
    }
}
