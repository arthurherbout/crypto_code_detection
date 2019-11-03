#include <cstdio>
#include <algorithm>

using namespace std;

int n, ans;
int a[1 << 4];

void dfs(int x)
{
    int i, j, k, l;
    
    for (i = 0; i < (1 << n); i++) {
        if (a[i] != i + 1) break;
    }
    
    if (i == (1 << n)) {
        ans++;
        
        return;
    }
    
    for (i = 0; i < n; i++) {
        if ((x >> i) & 1) continue;
        
        for (j = 0; j < (1 << n); j += (1 << i)) {
            for (k = j + (1 << i); k < (1 << n); k += (1 << i)) {
                for (l = 0; l < (1 << i); l++) swap(a[j + l], a[k + l]);
                
                dfs(x | (1 << i));
                
                for (l = 0; l < (1 << i); l++) swap(a[j + l], a[k + l]);
            }
        }
    }
}

int main()
{
    int t, i;
    
    scanf("%d", &t);
    
    for (i = 0; i < t; i++) {
        int j;
        
        scanf("%d", &n);
        
        for (j = 0; j < (1 << n); j++) scanf("%d", &a[j]);
        
        ans = 0;
        
        dfs(0);
        
        printf("Case #%d: %d\n", i + 1, ans);
    }
    
    return 0;
}
