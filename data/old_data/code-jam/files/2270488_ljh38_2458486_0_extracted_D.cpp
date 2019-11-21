#include <iostream>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

struct Chest {
    Chest() : LockType(0), Opened(false) {}
    int LockType;
    std::vector<int> Contents;
    bool Opened;
};

#define NUM_BITS (1 << 20)
bitset<NUM_BITS> visited;
vector<Chest> chests;

// Totally crappy brute force backtracking solution!!
// The only extra thing is keeping track of which subsets of chests we've
// already tried (in the 'visited' flags), saves trying multiple orderings
// to reach the same state.
bool solve(map<int, int>& keys, unsigned int visitedIndex, vector<int>& solution) {
    if (solution.size() == chests.size()) {
        return true;
    }

    visited.set(visitedIndex);
    
    for (size_t i = 0; i != chests.size(); ++i) {
        if (chests[i].Opened) continue; // already got this chest
        if (keys[chests[i].LockType] == 0) continue; // can't open it
        if (visited[visitedIndex | (1 << i)]) continue; // already reached the combination of chests if we tried this one

        // try opening this one
        map<int, int> newKeys = keys;
        solution.push_back(i + 1);
        newKeys[chests[i].LockType]--;
        for (size_t j = 0; j != chests[i].Contents.size(); ++j) {
            newKeys[chests[i].Contents[j]]++;
        }
        chests[i].Opened = true;
        if (solve(newKeys, visitedIndex | (1 << i), solution)) {
            return true;
        }

        // failed, roll back
        chests[i].Opened = false;
        solution.pop_back();
    }

    return false;
}

int main()
{
    istream& input = cin;

    int T;
    input >> T;
    for (int t = 0; t != T; ++t) {
        int K, N = 0;
        input >> K >> N;

        map<int, int> keys;
        for (int k = 0; k != K; ++k) {
            int key = 0;
            input >> key;
            keys[key]++;
        }

        chests.clear();
        visited.reset();
        
        for (int n = 0; n != N; ++n) {
            Chest chest;
            int Ki = 0;
            input >> chest.LockType >> Ki;
            for (int i = 0; i != Ki; ++i) {
                chest.Contents.push_back(0);
                input >> chest.Contents.back();
            }

            chests.push_back(chest);
        }

        vector<int> solution;
        bool solvable = solve(keys, 0, solution);
        if (solvable) {
            cout << "Case #" << t + 1 << ": ";
            for (size_t i = 0; i != solution.size(); ++i) {
                cout << solution[i] << " ";
            }
            cout << endl;
        } else {
            cout << "Case #" << t + 1 << ": IMPOSSIBLE\n";
        }
    }
    return 0;
}
