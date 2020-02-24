#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>
#include <limits>
#include <queue>
#include <algorithm>
#include <set>
#include <math.h>

using namespace std;

struct NumIdx {
    int a;
    int idx;

    inline bool operator <(const NumIdx & other) const {
        return a < other.a;
    }
};

int minSwapsToSort(vector<int>& v, int beg, int end) {
    int swaps = 0;
    for (int outer = beg; outer != end; ++outer) {
        int mini = numeric_limits<int>::max();
        int minIdx = -1;
        for (int i = outer; i != end; ++i) {
            if (v[i] < mini) {
                mini = v[i];
                minIdx = i;
            }
        }
        if (minIdx != outer) {
            ++swaps;
            swap(v[outer], v[minIdx]);
        }
    }
    return swaps;
}

int minSwapsToSortReverse(vector<int>& v, int beg, int end) {
    int swaps = 0;
    for (int outer = beg; outer != end; ++outer) {
        int maxi = numeric_limits<int>::min();
        int maxIdx = -1;
        for (int i = outer; i != end; ++i) {
            if (v[i] > maxi) {
                maxi = v[i];
                maxIdx = i;
            }
        }
        if (maxIdx != outer) {
            ++swaps;
            swap(v[outer], v[maxIdx]);
        }
    }
    return swaps;
}

// vector<int> swap1;
// vector<int> swap2;

bool isUpAndDown(const vector<int>& A) {
    bool goingUp = true;
    for (int i = 0; i+1 != A.size(); ++i) {
        if (goingUp) {
            if (A[i+1] < A[i]) {
                goingUp = false;
            }
        } else {
            if (A[i+1] > A[i]) {
                return false;
            }
        }
    }
    return true;
}

void minSwapsBrute(vector<int>& A, int elt, int numSwaps, int& solution) {
    if (elt == A.size()) {
        if (isUpAndDown(A)) {
            solution = min(solution, numSwaps);

            /*if (numSwaps == 2) {
                cout << "Solution: \n";
                assert(numSwaps == swap1.size());
                for (int i= 0; i != numSwaps; ++i) {
                    cout << swap1[i] << "<->" << swap2[i] << endl;
                }
                }*/
        }
        return;
    }

    for (int i = elt; i != A.size(); ++i) {
        swap(A[i], A[elt]);

        /*if (i != elt) {
            swap1.push_back(i);
            swap2.push_back(elt);
            }*/
        
        int thisSwap = (i - elt) + max(i - elt - 1, 0);
        minSwapsBrute(A, elt+1, numSwaps + thisSwap, solution);
        
        swap(A[i], A[elt]);

        /*if (i != elt) {
            swap1.pop_back();
            swap2.pop_back();
            }*/
    }
}

int minSwapsBrute(const vector<int>& A) {
    int solution = numeric_limits<int>::max();
    vector<int> A2 = A;
    minSwapsBrute(A2, 0, 0, solution);
    return solution;
}

int minSwaps(const vector<int>& A) {    
    // find the maximum element, has to be turning point of sequence
    int maxIdx = -1;
    int maxi = numeric_limits<int>::min();
    for (int i = 0; i != A.size(); ++i) {
        if (A[i] > maxi) {
            maxi = A[i];
            maxIdx = i;
        }
    }

    // try it in every position
    int mini = numeric_limits<int>::max();
    for (int i = 0; i != A.size(); ++i) {
        vector<int> B = A;
        int totalSwaps = 0;
        if (i != maxIdx) {
            ++totalSwaps;
            swap(B[i], B[maxIdx]);
        }
        totalSwaps += minSwapsToSort(B, 0, i);
        totalSwaps += minSwapsToSortReverse(B, i + 1, B.size());

        mini = min(mini, totalSwaps);
    }
    
    return mini;
}

int main() {
    // ifstream in("small1.in");
    istream& in = cin;

    int C;
    in >> C;
    for (int c = 0; c != C; ++c) {
        int N;
        in >> N;
        vector<int> A(N);
        for (int i = 0; i != N; ++i) {
            in >> A[i];
        }
        // assert(minSwaps(A) == minSwapsBrute(A));
        
        cout << "Case #" << (c+1) << ": " << minSwapsBrute(A) << endl;
    }
    return 0;
}
