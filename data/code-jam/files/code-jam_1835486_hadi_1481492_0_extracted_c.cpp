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
typedef pair<int64, int64> P;

int64 t, n, m, f;
vector<P> arr;
bool fail[300];

P solve(int64 m) {
    int64 fee = f;
    if(fee > m) return P(0, 0);
    m -= fee;
    int64 days = 0;
    int64 curday = 0;
    for(int i = 0; i < n; i ++) {
        int64 max_days = min(arr[i].second - curday + 1, m / arr[i].first);
        if(max_days == 0) break;
        days += max_days;
        curday += max_days;
        fee += arr[i].first * max_days;
        m -= arr[i].first * max_days;
    }
    return P(fee, days);
}

int main() {
    cin >> t;
    for(int I = 1; I <= t; I ++) {
        cin >> m >> f >> n;
        arr.resize(n);
        for(int i = 0; i < n; i ++) {
            cin >> arr[i].first >> arr[i].second;
            arr[i].second = -arr[i].second;
        }
        sort(arr.begin(), arr.end());
        memset(fail, 0, sizeof fail);
        for(int i = 0; i < n; i ++) {
            for(int j = i + 1; j < n; j ++) {
                if(-arr[j].second <= -arr[i].second || arr[i].first == arr[j].first) fail[j] = true;
            }
            if(arr[i].first > arr[0].first + f) fail[i] = true;
        }
        vector<P> v;
        for(int i = 0; i < n; i ++) {
            if(!fail[i]) {
                arr[i].second = -arr[i].second;
                v.push_back(arr[i]);
            }
        }
        /*for(int i = 0; i < v.size(); i ++) {
            cout << v[i].first << " " << v[i].second << endl;
        }*/
        arr = v;
        n = v.size();
        int64 result = 0;
        while(true) {
            P p = solve(m);
            if(p.second == 0) break;
            result += (m / p.first) * p.second;
            m %= p.first;
        }
        cout << "Case #" << I << ": " << result << endl;
    }
    return 0;
}