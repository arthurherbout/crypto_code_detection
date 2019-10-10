//
//  main.cpp
//  Myproject
//
//  Created by Jeffery Qi on 4/17/15.
//  Copyright (c) 2015 Jeffery Qi. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <utility>

using namespace std;

int main(int argc, const char * argv[]) {
    ifstream input;
    ofstream output;
    input.open("C-small-1-attempt0.in");
    output.open("result.txt");
    
    int n_test, n, count, pos, test_count = 0;
    long long spd;
    vector<pair<int, long long>> hikers;
    vector<pair<double, double>> ints;
    
    input >> n_test;
    
    while (n_test--) {
        input >> n;
        test_count++;
        hikers.clear();
        ints.clear();
        while (n--) {
            input >> pos >> count >> spd;
            for (int i = 0; i < count; i++) {
                hikers.push_back(make_pair(pos, spd + i));
                // cout << count << endl;
            }
        }
        
        for (pair<int, int> hiker : hikers) {
            double v_max = 360 / (double) (360 - hiker.first) * 360 / (double) hiker.second;
            double v_min = 360 / (double) (720 - hiker.first) * 360 / (double) hiker.second;
            ints.push_back(make_pair(v_min, v_max));
        }
        
        if (ints.size() == 1) {
            output << "Case #" << test_count << ": " << 0 << endl;
        } else {
            pair<double, double> p1, p2;
            p1 = ints[0];
            p2 = ints[1];
            if (p1.second < p2.first || p2.second < p1.first) {
                output << "Case #" << test_count << ": " << 1 << endl;
            } else {
                output << "Case #" << test_count << ": " << 0 << endl;
            }
        }
    }
    
    
    
    return 0;
}
