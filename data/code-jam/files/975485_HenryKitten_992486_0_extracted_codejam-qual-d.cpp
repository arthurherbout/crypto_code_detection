#include <cstdio>
#include <algorithm>

using namespace std;

struct Num{
    int val;
    int pos;
    bool operator<(const Num& num) const{
        return val < num.val;
    }
};

int main(){
    int ncas;
    scanf("%d",&ncas);
    for(int m=1;m<=ncas;m++){
        Num nums[2000];
        int n,nturn = 0;
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            int val;
            scanf("%d",&val);
            nums[i].val = val;
            nums[i].pos = i;
        }
        sort(nums,nums+n);

        bool used[2000] = {0};
        for(int i=0;i<n;i++){
            if(used[i]) continue;


            int pos = nums[i].pos;
            used[i] = 1;
            while(!used[pos]){
                used[pos] = 1;
                pos = nums[pos].pos;             
                nturn++;
            }
        }

        printf("Case #%d: %.6lf\n",m,nturn*2.0);
    }
    return 0;
}
