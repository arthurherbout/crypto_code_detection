#include <cstdio>
const int N = 1111;
char s[N];
int main(){
    int t;
    int nCase = 0;
    freopen("d:\\A-large.in","r",stdin);
    freopen("d:\\A-large.out","w",stdout);
    for (scanf("%d",&t);t;t--){
        int n;
        scanf("%d",&n);
        scanf("%s",s);
        int ans = 0;
        int sum = 0;
        for (int i=0;i<=n;i++){
            if (sum<i){
                ans += i-sum;
                sum = i;
            }
            sum += s[i]-'0';
        }
        printf("Case #%d: %d\n",++nCase,ans);
    }
    return 0;
}
