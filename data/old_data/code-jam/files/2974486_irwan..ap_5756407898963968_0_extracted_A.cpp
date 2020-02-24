#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
using namespace std;

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {

        int row1;
        cin >> row1;
        vector<vector<int>> a1(4, vector<int>(4));
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cin>> a1[i][j];
            }
        }
        set<int> set1(a1[row1-1].begin(), a1[row1-1].end());

        int row2;
        cin >> row2;
        vector<vector<int>> a2(4, vector<int>(4));
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cin >> a2[i][j];
            }
        }
        set<int> set2(a2[row2-1].begin(), a2[row2-1].end());

        vector<int> result;
        set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(result));

        cout << "Case #" << t << ": ";
        if (result.size() == 0) {
            cout << "Volunteer cheated!" << endl;
        } else if (result.size() == 1) {
            cout << result[0] << endl;
        } else {
            cout << "Bad magician!" << endl;
        }
    }

    return 0;
}

