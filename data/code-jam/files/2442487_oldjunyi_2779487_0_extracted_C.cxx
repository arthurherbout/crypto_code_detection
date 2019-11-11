#include <cstdio>
#include <vector>
#include <bitset>
#include <cstring>
#include <algorithm>
using namespace std;

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        int a[2005],b[2005],ans[2005],n,cnt=0;
        vector<int> u[2005];
        scanf("%d",&n);
        for(int i=0;i<n;i++) scanf("%d",a+i);
        for(int i=0;i<n;i++) scanf("%d",b+i);
        for(int x=1;x<=n;x++) for(int i=n-1;~i;i--)
            if(a[i]==x) ans[i]=++cnt;
        printf("Case #%d:",++no);
        for(int i=0;i<n;i++) printf(" %d",ans[i]);
        puts("");
    }
}
