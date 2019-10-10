#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cmath>
#include <set>
#include <map>
#include <queue>
using namespace std;

typedef long long int64;
typedef pair<int, int> P;

struct Level {
    int time;
    int prob;
    int idx;
    
    bool operator<(const Level& b) const {
        if(prob == 0 && b.prob == 0) {
            return idx < b.idx;
        }
        if(prob == 0) {
            return false;
        }
        if(b.prob == 0) {
            return true;
        }
        int p1 = time * b.prob;
        int p2 = b.time * prob;
        if(p1 == p2) {
            return idx < b.idx;
        }
        return p1 < p2;
    }
} arr[2000];

int n, t;

int main() {
    cin >> t;
    for(int I = 1; I <= t; I ++) {
        cin >> n;
        for(int i = 0; i < n; i ++) {
            cin >> arr[i].time;
            arr[i].idx = i;
        }
        for(int i = 0; i < n; i ++) {
            cin >> arr[i].prob;
        }
        sort(arr, arr + n);
        cout << "Case #" << I << ":";
        for(int i = 0; i < n; i ++) {
            cout << " " << arr[i].idx;
        }
        cout << endl;
    }
    return 0;
}