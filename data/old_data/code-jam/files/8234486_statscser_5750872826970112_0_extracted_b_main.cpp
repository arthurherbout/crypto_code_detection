//
//  main.cpp
//  Skeleton
//
//  Created by Yang Zhao on 4/25/15.
//  Copyright (c) 2015 Stanford University. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iomanip>
#include <cmath>
#define forn for(int i = 0; i < n; ++i)

using namespace std;

int main(int argc, const char * argv[]) {
    ifstream input("input.txt");
    ofstream output("output.txt");
    output.setf(ios::fixed, ios::floatfield);
    int t, n;
    int precision = 10;
    double v, x;
    input >>t;
    for (int i = 0; i < t; ++i) {
        input >>n >>v >>x;
        vector<double> r, c;
        double r_tmp, c_tmp;
        for (int j = 0; j < n; ++j) {
            input >>r_tmp >>c_tmp;
            r.push_back(r_tmp);
            c.push_back(c_tmp);
        }
        output <<"Case #" <<i+1 <<": ";
        double c_max = 0.0, c_min = 100.0;
        for (int j = 0; j < n; ++j) {
            c_max = (c[j] > c_max) ? c[j] : c_max;
            c_min = (c[j] < c_min) ? c[j] : c_min;
        }
        if (x > c_max || x < c_min) {
            output <<"IMPOSSIBLE" <<endl;
            continue;
        }
        if (n == 1) {
            if (c[0] == x) {
                output <<setprecision(precision) <<v/r[0] <<endl;
            } else {
                output <<"IMPOSSIBLE" <<endl;
            }
        } else {
            vector<double> p;
            for (int j = 0; j < n; ++j) p.push_back(c[j]*r[j]);
            if (c[0] == c[1]) {
                if (x == c[0]) {
                    output <<setprecision(precision) <<v / (r[0] + r[1]) <<endl;
                } else {
                    output <<"IMPOSSIBLE" <<endl;
                }
            } else {
                double t1 = (c[1] - x) * v / r[0] / (c[1] - c[0]);
                double t2 = (x - c[0]) * v / r[1] / (c[1] - c[0]);
                double t_max = (t1 > t2) ? t1 : t2;
                output <<setprecision(precision) <<t_max <<endl;
            }
        }
    }
    output.close();
    input.close();
    return 0;
}
