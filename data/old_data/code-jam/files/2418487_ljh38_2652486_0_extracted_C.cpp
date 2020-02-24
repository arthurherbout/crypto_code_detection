#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>

using namespace std;

typedef vector<int> Set;

void makeSets(int elt, int N, int M, vector<Set>& sets, Set building)
{
    if (elt > N) {
        sets.push_back(building);
        return;
    }
    
    building.push_back(1);
    for (int i = 2; i <= M; ++i) {
        building.back() = i;
        makeSets(elt + 1, N, M, sets, building);
    }
}

void printSet(const Set& s) {
    for (int i = 0; i != s.size(); ++i) {
        cout << s[i];
    }
    cout << endl;
}

typedef map<int, double> ProbTable;

void calcProbs(int idx, int prod, const Set& s, map<int, double>& probs) {
    if (idx == s.size()) {
        probs[prod]++;
    } else {
        calcProbs(idx + 1, prod, s, probs);
        calcProbs(idx + 1, prod * s[idx], s, probs);
    }
}
// compute map from product -> probability of that product, given set s
void calcProbs(const Set& s, map<int, double>& probs) {
    calcProbs(0, 1, s, probs);

    double denom = 1.0 / double(1 << s.size());
    for (map<int, double>::iterator itr = probs.begin(); itr != probs.end(); ++itr) {
        itr->second *= denom;
    }
}

int main()
{
    istream& input = cin;
    // ifstream input("sample");
    int T;
    input >> T;
    for (int t = 0; t != T; ++t) {
        int R, N, M, K;
        input >> R >> N >> M >> K;

        cout << "Case #1:\n";
        // generate all N-subsets of 2...M
        vector<Set> sets;
        makeSets(1, N, M, sets, Set());

        vector<ProbTable> probTables;
        probTables.resize(sets.size());
        for (int i = 0; i != sets.size(); ++i) {
            calcProbs(sets[i], probTables[i]);
        }
        
        for (int r = 0; r != R; ++r) {

            vector<double> probs(sets.size(), 1.0);
            for (int k = 0; k != K; ++k) {
                int product;
                input >> product;

                for (int s = 0; s != sets.size(); ++s) {
                    probs[s] *= probTables[s][product];
                }
            }

            int best = -1;
            double bestProb = 1.0;
            for (int s = 0; s != sets.size(); ++s) {
                if (best == -1 || probs[s] > bestProb) {
                    bestProb = probs[s];
                    best = s;
                }
            }

            printSet(sets[best]);
        }
    }
    return 0;
}
