//
//  main.cpp
//  Logging
//
//  Created by Yang Zhao on 4/17/15.
//  Copyright (c) 2015 Stanford University. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Point {
    int x, y;
    Point() {}
    Point(int a, int b): x(a), y(b) {}
};

int main(int argc, const char * argv[]) {
    ifstream input("small_in.txt");
    ofstream output("small_out.txt");
    int t, n, x, y;
    input >>t;
    for (int i = 0; i < t; ++i) {
        input >>n;
        vector<Point> points;
        for (int j = 0; j < n; ++j) {
            input >>x >>y;
            points.push_back(Point(x, y));
        }
        output <<"Case #" <<i+1 <<":" <<endl;
        if (n <= 3) {
            for (int j = 0; j < n; ++j) {
                output <<0 <<endl;
            }
            continue;
        }
        for (int j = 0; j < n; ++j) {
            int minimum = n;
            for (int k = 0; k < n; ++k) {
                if (j != k) {
                    long long delta_x = points[k].x - points[j].x;
                    long long delta_y = points[k].y - points[j].y;
                    int positive = 0, negative = 0;
                    for (int l = 0; l < n; ++l) {
                        long long value = (long long)(points[l].x - points[j].x) * delta_y - (long long)(points[l].y - points[j].y) * delta_x;
                        if (value > 0) positive++;
                        if (value < 0) negative++;
                    }
                    minimum = min(min(positive, negative), minimum);
                    if (minimum == 0) {
                        break;
                    }
                }
            }
            output <<minimum <<endl;
        }
    }
    output.close();
    input.close();
    return 0;
}
