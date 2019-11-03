#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int findPoints(const vector<double>& naomi, const vector<double>& ken) {
    
    int ans = 0;
    int n = naomi.size();

    vector<bool> used(n, false);

    for (int i = 0; i < n; i++) {
        bool canCounter = false;
        // Find the cheapest unused block that can counter Naomi's block
        for (int j = 0; j < n; j++) {
            if (!used[j] && ken[j] > naomi[i]) {
                canCounter = true;
                used[j] = true;
                break;
            }
        }
        if (!canCounter) {
            // Naomi gains 1 point
            ans++;
            // Find the cheapest block to be thrown away
            for (int j = 0; j < n; j++) {
                if (!used[j]) {
                    used[j] = true;
                    break;
                }
            }
        }
    }

    return ans;
}

int findPointsCheating(vector<double> naomi, vector<double> ken) {

    int n = naomi.size();
    int ans = 0;

    for (int i = 0; i < n; i++) {
        if (naomi[i] < ken[0]) {
            // This block can't win. Use it to waste Ken's largest block
            ken.erase(ken.begin() + (ken.size()-1));
        } else {
            // This block can win 1 point
            ans++;
            ken.erase(ken.begin());
        }
    }

    return ans;
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int n;
        cin >> n;
        vector<double> naomi(n);
        for (int i = 0; i < n; i++) {
            cin >> naomi[i];
        }
        vector<double> ken(n);
        for (int i = 0; i < n; i++) {
            cin >> ken[i];
        }

        sort(naomi.begin(), naomi.end());
        sort(ken.begin(), ken.end());

        int pointsNormal = findPoints(naomi, ken);
        int pointsCheating = findPointsCheating(naomi, ken);
        cout << "Case #" << t << ": " << pointsCheating << " " << pointsNormal << endl;
    }

    return 0;
}

