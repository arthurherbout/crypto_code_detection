//
//  main.cpp
//  cj2016_r1_a
//
//  Created by DongJiaming on 4/15/16.
//  Copyright © 2016 NYU. All rights reserved.
//

#include <iostream>

using namespace std;

int main() {
    
    freopen("/Users/JiamingDong/Downloads/A-large.in-2.txt", "r", stdin);
    freopen("/Users/JiamingDOng/Documents/cj2016/r1/A-large.out", "w", stdout);
    
    int n;
    cin >> n;
    for (int tt = 1; tt <= n; tt++) {
        string s;
        cin >> s;
        string ans = "";
        ans = s[0];
        for (int i = 1; i < s.length(); i++) {
            if (s[i] >= ans[0]) {
                ans = s[i] + ans;
            } else {
                ans = ans + s[i];
            }
        }
        cout << "Case #" << tt << ": " << ans << endl;
    }
    return 0;
}
