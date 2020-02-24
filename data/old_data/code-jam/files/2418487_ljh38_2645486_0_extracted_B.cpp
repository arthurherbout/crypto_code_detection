#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>

using namespace std;


int v[10];
int max_gain[10][6];

int solve(int E, int R, int N) {
    for (int e = 0; e <= E; ++e) {
        max_gain[N - 1][e] = e * v[N - 1];
    }
            
    for (int i = N - 2; i >= 0; --i) {
        for (int energy = 0; energy <= E; ++energy) {
            max_gain[i][energy] = 0;
            for (int use = 0; use <= energy; ++use) {
                int newEnergy = min(R + energy - use, E);
                int gain = use * v[i] + max_gain[i + 1][newEnergy];
                if (gain > max_gain[i][energy]) {
                    max_gain[i][energy] = gain;
                }
            }
        }
    }
    
    return max_gain[0][E];
}

int main()
{
    istream& input = cin;
    // ifstream input("sample");
    int T;
    input >> T;
    for (int t = 0; t != T; ++t) {
        int E, R, N;
        input >> E >> R >> N;
        for (int n = 0; n != N; ++n) {
            input >> v[n];
        }
        cout << "Case #" << (t + 1) << ": " << solve(E, R, N) << endl;
    }
    return 0;
}
