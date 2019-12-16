#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

int n;
vi bff;

bool is_valid(vi& trace){
    int m = trace.size();
    for(int i=0;i<m;i++){
        bool valid = 
            (bff[trace[i]] == trace[(i-1+m)%m]) ||
            (bff[trace[i]] == trace[(i+1)%m]);
        if(!valid) return false;
    }
    return true;
}

int max_circle(int selected, vi& trace){
    if(selected == (1<<n-1)) return 0;
    int ret = 0;
    if(is_valid(trace)) ret = trace.size();
    for(int i=0;i<n;i++){
        bool vis = (selected >> i) & 1;
        if(!vis){
            trace.push_back(i);
            ret = max(ret,
                    max_circle(selected | (1 << i), trace));
            trace.pop_back();
        }
    }
    return ret;
}

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start %d\n", ti);
        scanf("%d", &n);
        bff = vi(n);
        for(int i=0;i<n;i++){
            scanf("%d", &bff[i]);
            bff[i] --;
        }
        vi trace;
        printf("Case #%d: %d\n", ti, max_circle(0, trace));
        fprintf(stderr, "end %d\n", ti);
    }
    return 0;
}
