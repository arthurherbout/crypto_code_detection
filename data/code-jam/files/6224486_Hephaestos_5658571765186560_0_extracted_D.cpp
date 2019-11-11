#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>

using namespace std;


void main(){
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i){
        int X, R, C;
        cin >> X >> R >> C;
        string r;
        switch (X){
        case 1:
            r = "GABRIEL";
            break;
        case 2:
            if ((R*C) % 2 == 0) r = "GABRIEL";
            else r = "RICHARD";
            break;
        case 3:
            if (R == 1 || C == 1){
                r = "RICHARD";
                break;
            }
            if ((R*C) % X == 0) r = "GABRIEL";
            else r = "RICHARD";
            break;
        case 4:            
            if (min(R, C) <= 2){
                r = "RICHARD";
                break;
            }
            if ((R*C) % X == 0) r = "GABRIEL";
            else r = "RICHARD";
            break;
        /*case 5:
            if (min(R,C)<=3)*/
        }

        cout << "Case #" << i + 1 << ": " << r.c_str() << endl;
    }
}