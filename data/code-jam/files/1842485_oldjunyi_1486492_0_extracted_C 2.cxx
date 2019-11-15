#include <cstdio>
#include <vector>
#include <string>
#include <numeric>
#include <cstring>
#include <cassert>
#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <algorithm>
using namespace std;

typedef pair<int,int> PII;
typedef long long LL;
const int INF = 1000000007;

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        int n,p[2005],h[2005];
        scanf("%d",&n);
        for(int i=1;i<n;i++){
            scanf("%d",p+i);
            h[i]=500000000+i*250000;
        }
        h[n]=500000000+n*250000;
        bool ok=false;
        int cnt=0;
        while(!ok){
            if(cnt++>1000000) break;
            ok=true;
            int i,j;
            for(i=1;i<n;i++){
                for(j=i+1;j<p[i];j++){
                    //printf("step1 %d %d -> %I64d %I64d\n",i,j,LL(h[j]-h[i])*(p[i]-i),LL(h[p[i]]-h[i])*(j-i));
                    if(LL(h[j]-h[i])*(p[i]-i)>=LL(h[p[i]]-h[i])*(j-i)) break;
                }
                if(j<p[i]) break;
                for(j=p[i]+1;j<=n;j++){
                    //printf("step2 %d %d -> %I64d %I64d\n",i,j,LL(h[j]-h[i])*(p[i]-i),LL(h[p[i]]-h[i])*(j-i));
                    if(LL(h[j]-h[i])*(p[i]-i)> LL(h[p[i]]-h[i])*(j-i)) break;
                }
                if(j<=n) break;
            }
            //printf("i = %d\n",i);
            if(i<n){
                ok=false;
                for(j=i+1;j<p[i];j++){
                    LL c=LL(h[p[i]]-h[i])*(j-i);
                    if(LL(h[j]-h[i])*(p[i]-i)>=c){
                        LL v=c/(p[i]-i)+3;
                        while(v*(p[i]-i)>=c) v--;
                        h[j]=v+h[i];
                    }
                }
                for(j=p[i]+1;j<=n;j++){
                    LL c=LL(h[p[i]]-h[i])*(j-i);
                    if(LL(h[j]-h[i])*(p[i]-i)> c){
                        LL v=c/(p[i]-i)+3;
                        while(v*(p[i]-i)> c) v--;
                        h[j]=v+h[i];
                    }
                }
            }
        }
        printf("Case #%d:",++no);
        if(!ok){
            puts(" Impossible");
            continue;
        }
        for(int i=1;i<=n;i++) printf(" %d",h[i]);
        puts("");
    }
}
