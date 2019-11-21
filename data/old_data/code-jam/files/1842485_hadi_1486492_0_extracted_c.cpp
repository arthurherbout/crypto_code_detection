#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <cmath>
using namespace std;

typedef pair<int, int> P;

const int N = 20000;
const double eps = 1e-8;

int T, arr[N], res[N], n;

bool visible(int a, int b) {
    for(int i = a + 1; i < b; i ++) {
        double y = res[a] + (double)(res[b] - res[a]) * (double)(i - a) / (b - a);
        if(y < res[i] + eps) return false;
    }
    return true;
    
}

bool check() {
    for(int i = 0; i + 1 < n; i ++) {
        int r = i + 1;
        for(int j = i + 2; j < n; j ++) {
            if(visible(i, j))
                r = j;
        }
        if(arr[i] != r) {
            cerr << "Error#" << i << " expected " << arr[i] << " but " << r << endl;
            return false;
        }
    }
    return true;
}

bool solve(int a, int b) {
    if(a + 1 == b) return true;
    int X = res[a] - 1;
    int cur = a + 1;
    vector<int> v;
    while(cur < b) {
        if(arr[cur] > b) return false;
        v.push_back(cur);
        cur = arr[cur];
    }
    for(int i = 0; i < v.size(); i ++) {
        res[v[v.size()-1-i]] = res[b] - i - 1;
    }
    cur = a + 1;
    while(cur < b) {
        if(!solve(cur, arr[cur])) return false;
        cur = arr[cur];
    }
    return true;
}

bool solve() {
    //cerr << "solve()" << endl;
    int X = 1000000000;
    int cur = 0;
    while(cur < n - 1) {
      //  cerr << "cur1 = " << cur << endl;
        res[cur] = X;
        res[arr[cur]] = X;
        if(!solve(cur, arr[cur])) {
            return false;
        }
        cur = arr[cur];
    }
    return true;
}

int main() {
    cin >> T;
    for(int I = 1; I <= T; I ++) {
        cin >> n;
        for(int i = 0; i < n - 1; i ++) {
            cin >> arr[i];
            arr[i] --;
        }
        cout << "Case #" << I << ":";
        if(!solve()) {
            cout << " Impossible" << endl;
        } else {
            cerr << (check() ? "OK" : "NOK") << endl;
            for(int i = 0; i < n; i ++) {
                cout << " " << res[i];
            }
            cout << endl;
        }
    }
    return 0;
}
