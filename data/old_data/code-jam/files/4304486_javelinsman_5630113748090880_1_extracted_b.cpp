#include <cstdio>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

const int INF = 987654321;

bool has_selected(vvi& grp, int k, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<2;j++){
            if(grp[i][j] != -1 && grp[i][j] == k) return true;
        }
    }
    return false;
}

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start %d\n", ti);
        int n; scanf("%d", &n);
        vvi seg(2*n-1, vi(n));
        for(int i=0;i<2*n-1;i++){
            for(int j=0;j<n;j++){
                scanf("%d", &seg[i][j]);
            }
        }
        vvi grp(n, vi(2, -1));
        int tgt = -1;
        for(int i=0;i<n;i++){
            int min_val[2] = {INF, INF}, min_ind[2] = {-1, -1}; 
            for(int j=0;j<2*n-1;j++){
                if(has_selected(grp, j, n)) continue;
                if(min_val[0] >= seg[j][i]){
                    min_val[1] = min_val[0];
                    min_ind[1] = min_ind[0];
                    min_val[0] = seg[j][i];
                    min_ind[0] = j;
                }
            }
            grp[i][0] = min_ind[0];
            if(min_val[0] == min_val[1]) grp[i][1] = min_ind[1];
            else tgt = i;
        }
        vi ans(n);
        for(int i=0;i<n;i++){
            if(i == tgt) ans[i] = seg[grp[tgt][0]][tgt];
            else{
                if(seg[grp[tgt][0]][i] == seg[grp[i][0]][tgt]){
                    ans[i] = seg[grp[i][1]][tgt];
                }
                else{
                    ans[i] = seg[grp[i][0]][tgt];
                }
            }
        }
        printf("Case #%d: ", ti);
        for(int i=0;i<n;i++){
            printf("%d ", ans[i]);
        }
        printf("\n");
        fprintf(stderr, "end %d\n", ti);
    }
    return 0;
}
