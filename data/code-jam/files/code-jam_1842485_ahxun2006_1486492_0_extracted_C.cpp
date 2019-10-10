#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

int N;
int X[2005], height[2005];
bool impossible;

void solve(int a, int b, int h) {
    if (impossible || a > b) return;
    int m = X[a];
    if (height[m] == -1) height[m] = h;
    if (height[a] == -1) height[a] = h - 1;
    if (height[b] == -1) height[b] = h - 1;
    if (a == b) return;
    //cout << m << " " << a << " " << b << endl;
    //for (int i = 0; i < N; ++i) cout << height[i] << " " ; cout << endl;
    //system("pause"); 
    if (m > b) {
        solve(a, b - 1, h - 1);
        return;
    }    
    if (m != b) {
        solve(m, b, h - 1);
        solve(a, m, h - 1);
    } else {
        for (int i = a + 1; i < b; ++i)
            if (X[i] == b) {
                solve(i, b, h - 1);
                solve(a, i, h - 1);
                break;
            }    
    }        
}    
    
int main() {
    freopen("C-small-attempt0.in", "r", stdin);
    freopen("C-small-attempt0.out", "w", stdout);
    int T;
    scanf("%d", &T);
    for (int caseId = 1; caseId <= T; ++caseId) {
        scanf("%d", &N);
        for (int i = 0; i + 1 < N; ++i) {
            scanf("%d", &X[i]);
            --X[i];
        }    
        memset(height, -1, sizeof(height));
        printf("Case #%d:", caseId);
        solve(0, N - 1, 1000000000);
        bool impossible = false;
        for (int i = 0; i < N; ++i) if (height[i] == -1) impossible = true;
        if (impossible) {//!check(0, N - 1)
            printf(" Impossible\n");
        }  else {
            for (int i = 0; i < N; ++i) {
                printf(" %d", height[i]);
            }    
            printf("\n");
        }    
    }    
   // while (1);
    //system("pause");
    return 0;
}    
