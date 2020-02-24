//
//  main.cpp
//  Haircut
//
//  Created by Yang Zhao on 4/17/15.
//  Copyright (c) 2015 Stanford University. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>
using namespace std;

struct classcomp {
    bool operator() (const pair<long, int>& lhs, const pair<long, int>& rhs) const
    {
        if (lhs.first == rhs.first) return lhs.second < rhs.second;
        return lhs.first < rhs.first;
    }
};

int main(int argc, const char * argv[]) {
    ifstream input("small_in.txt");
    ofstream output("small_out.txt");
    int t, b, n, tmp;
    input >>t;
    for (int i = 0; i < t; ++i) {
        input >>b >>n;
        vector<int> time(b, 0);
        double t_inverse = 0.0;
        for (int j = 0; j < b; ++j) {
            input >>tmp;
            time[j] = tmp;
            t_inverse += 1.0 / (double)tmp;
        }
        set<pair<long, int>, classcomp> tree;
        double amount = (double)n / t_inverse;
        for (int j = 0; j < b; ++j) {
            int r = floor(amount / (double)time[j]);
            n -= r;
            tree.insert(pair<long, int>(r * time[j], j+1));
        }
        int minimum = INT_MAX, id = 0;
        if (n == 0) {
            for (int j = 0; j < b; ++j) {
                if (time[j] <= minimum) {
                    minimum = time[j];
                    id = j+1;
                }
            }
            output <<"Case #" <<i+1 <<": " <<id <<endl;
            continue;
        }
        while (--n >= 1) {
            pair<long, int> newpair = *tree.begin();
            newpair.first += time[newpair.second - 1];
            tree.erase(tree.begin());
            tree.insert(newpair);
        }
        output <<"Case #" <<i+1 <<": " <<tree.begin()->second <<endl;
    }
    output.close();
    input.close();
    return 0;
}
