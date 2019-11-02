#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>
#include <opencv2/opencv.hpp>

using namespace std;

vector<int> Q = { 1, 2, 3, 4, 
                 2, -1, 4, -3,
                 3, -4, -1,2,
                 4, 3, -2, -1};

int F(int a, int b) {
    bool s = (a > 0 == b > 0); //s true if positive
    a = abs(a)-1;
    b = abs(b)-1;
    int r = Q[4 * a + b];
    if (!s) r *= -1;
    return r;
}




void main(){
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i){
        int L;
        unsigned long long X;
        cin >> L >> X;
        if (X>11)
            X = (X - 11) % 4 + 11;
        vector<int> S(L*X);
        cv::Mat Xj = cv::Mat::zeros(L*X, L*X, CV_16S);
        for (int j = 0; j < L; ++j){
            char c;
            cin >> c;
            int n = static_cast<int>(c - 103);
            for (int k = 0; k < X; ++k) {
                S[k*L + j] = n;
            }
        }
        vector<int> Is;
        vector<int> Ks;
        int xi = 1;
        int xk = 1;
        for (int j = 0; j < L*X; ++j){
            xi = F(xi, S[j]);
            if (xi == 2) Is.push_back(j);
            xk = F(S[L*X - 1 - j],xk);
            if (xk == 4) Ks.push_back(L*X - 1 - j);
            int xj = 1;
            for (int k = j + 1; k < L*X; ++k){
                Xj.at<int16_t>(j, k) = xj;
                xj = F(xj, S[k]);
            }
        }
        string r = "NO";
        bool found = false;
        for (auto xi : Is){
            for (auto xk : Ks){
                if (xk - xi < 1) break;
                int xj = 1;
                /*for (auto j = xi + 1; j < xk; ++j){
                    xj = F(xj, S[j]);
                }*/
                if (Xj.at<int16_t>(xi, xk)  == 3) {
                    found = true;
                    r = "YES";
                    break;
                }
            }
            if (found) break;
        }
        cout << "Case #" << i + 1 << ": " << r.c_str() << endl;
    }
}