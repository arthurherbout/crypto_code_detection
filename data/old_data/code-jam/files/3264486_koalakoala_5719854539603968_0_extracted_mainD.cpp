//
//  mainD.cpp
//  CJ
//
//  Created by Brian Lui on 8/4/2017.
//  Copyright © 2017 Brian Lui. All rights reserved.
//

#include <stdio.h>

#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
    int t;
    cin >> t;
    int caseN = 1;
    int n, m, r, c, y, z;
    char ch;
    vector< vector<int> > grid;
    vector< vector<int> > result;
    
    
    while (t--) {
        y = 0;
        z = 0;
        
        grid.clear();
        result.clear();
        
        cin >> n >> m;
        grid.resize(n);
        for (int i = 0; i < n; i++) {
            grid[i].resize(n, '.');
        }
        
        for (int i = 0; i < m; i++) {
            cin >> ch >> r >> c;
            grid[r - 1][c - 1] = ch;
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 'o') {
                    char original = grid[i][j];
                    bool plus = false;
                    for (int k = 0; k < n; k++) {
                        if (k == j) continue;
                        if ((grid[i][k] == 'x') || (grid[i][k] == 'o')) {
                            plus = true;
                            break;
                        }
                    }
                    
                    for (int k = 0; k < n; k++) {
                        if (k == i) continue;
                        if ((grid[k][j] == 'x') || (grid[k][j] == 'o')) {
                            plus = true;
                            break;
                        }
                    }
                    
                    bool cross = false;
                    if (i + j <= n - 1) {
                        for (int k = 0; i + j - k >= 0; k++) {
                            if (k == j) continue;
                            if ((grid[i + j - k][k] == '+') || (grid[i + j - k][k] == 'o')) {
                                cross = true;
                                break;
                            }
                        }
                    } else {
                        for (int k = n - 1; i + j - k < n; k--) {
                            if (k == j) continue;
                            if ((grid[i + j - k][k] == '+') || (grid[i + j - k][k] == 'o')) {
                                cross = true;
                                break;
                            }
                        }
                    }
                    
                    if (i <= j) {
                        for (int k = 0; -i + j + k < n; k++) {
                            if (k == i) continue;
                            if ((grid[k][-i + j + k] == '+') || (grid[k][-i + j + k] == 'o')) {
                                cross = true;
                                break;
                            }
                        }
                    } else {
                        for (int k = n - 1; -i + j + k >= 0; k--) {
                            if (k == i) continue;
                            if ((grid[k][-i + j + k] == '+') || (grid[k][-i + j + k] == 'o')) {
                                cross = true;
                                break;
                            }
                        }
                    }
                    
                    if (plus && cross) {
                        continue;
                    } else if (plus) {
                        grid[i][j] = '+';
                    } else if (cross) {
                        grid[i][j] = 'x';
                    } else {
                        grid[i][j] = 'o';
                    }
                    
                    if (grid[i][j] != original) {
                        z++;
                        vector<int> v = {grid[i][j], i + 1, j + 1};
                        result.push_back(v);
                    }
                }
            }
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if ((grid[i][j] == '+') || (grid[i][j] == 'x')) {
                    y++;
                } else if (grid[i][j] == 'o') {
                    y += 2;
                }
            }
        }
        
        cout << "Case #" << caseN++ << ": " << y << " " << z << endl;
        for (vector< vector<int> >::iterator it = result.begin(); it != result.end(); ++it) {
            char ch = (*it)[0];
            cout << ch << " " << (*it)[1] << " " << (*it)[2] << endl;
        }
    }
    
    
    return 0;
}
