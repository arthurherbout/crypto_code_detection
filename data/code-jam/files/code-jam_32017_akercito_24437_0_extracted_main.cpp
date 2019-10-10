// Problem A

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <set>

using namespace std;

int main () {

    ifstream in("A.in");
    ofstream out("A.out");
    int cases;
    in >> cases;
    long long n, A, B, C, D, x0, y0, M;
    for (int c = 0; c < cases; c++) {
        in >> n >> A >> B >> C >> D >> x0 >> y0 >> M;
        vector <long long> XX, YY;
        int X = x0;
        int Y = y0;
        XX.push_back(X);
        YY.push_back(Y);
        for (int i = 1; i <= n - 1; i++) {
            X = (A * X + B) % M;
            Y = (C * Y + D) % M;
            XX.push_back(X);
            YY.push_back(Y);
        }
        int count = 0;
        for (int i = 0; i < XX.size(); i++) {
            for (int j = i + 1; j < XX.size(); j++) {
                for (int k = j + 1; k < XX.size(); k++) {
                    if (((XX[i] + XX[j] + XX[k]) % 3) != 0) continue;
                    if (((YY[i] + YY[j] + YY[k]) % 3) != 0) continue;
                    count++;
                }
            }
        }
        out << "Case #" << (c + 1) << ": " << count << endl;
    }
    in.close();
    out.close();

}