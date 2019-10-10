#include <set>
#include <map>
#include <queue>
#include <vector>
#include <string>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <algorithm>

using namespace std;


int main() {
    freopen("A-large.in", "r", stdin);
    freopen("A-large.out", "w", stdout);

    int T;
    scanf("%d", &T);
    //cout << "T= " << T << endl;
    for(int t = 0; t < T;++t) {
        int n;
        string peo;
        cin >> n >> peo;
        //cout <<"see " << n << " " << peo << endl;
        int ans = 0, sum = 0;
        for(size_t i = 0;i < peo.size();++i) {
            if(sum < i) {
                ans += (i - sum);
                sum += int(peo[i] - '0') + (i - sum);
            } else {
                sum += int(peo[i] - '0');
            }
        }
        printf("Case #%d: %d\n", t + 1, ans);
    }

    return 0;
}
