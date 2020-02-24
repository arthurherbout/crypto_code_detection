#include <algorithm>
#include <iostream>
#include <vector>

struct activity {
    int start;
    int end;
    bool operator<(const activity& other) const {
        return start < other.start;
    }
};

int solve_one(std::vector<activity> A, std::vector<activity> B) {

    int Na = A.size();
    int Nb = B.size();

    sort(A.begin(), A.end());
    sort(B.begin(), B.end());

    int totalA = 0;
    for (int i = 0; i < Na; i++) {
        totalA += A[i].end - A[i].start;
    }
    int gapA = 720 - totalA;

    std::vector<int> gaps;
    if (Na > 1) {
        for (int i = 0; i < Na-1; i++) {
            bool can_close = true;
            for (int j = 0; j < Nb; j++) {
                if (B[j].start >= A[i].end && B[j].start <= A[i+1].start) {
                    can_close = false;
                    break;
                }
            }
            if (can_close) {
                gaps.push_back(A[i+1].start-A[i].end);
            }
        }
        bool can_close = true;
        for (int j = 0; j < Nb; j++) {
            if (B[j].start >= A[Na-1].end || B[j].end <= A[0].start) {
                can_close = false;
                break;
            }
        }
        if (can_close) {
            gaps.push_back(A[0].start+24*60-A[Na-1].end);
        }
    }
    sort(gaps.begin(), gaps.end());

    for (int i = 0; i < gaps.size(); i++) {
        if (gaps[i] <= gapA) {
            gapA -= gaps[i];
            gaps[i] = 0;
        }
    }
    
    int ans = Na;
    for (int i = 0; i < gaps.size(); i++) {
        if (gaps[i] == 0) {
            ans--;
        }
    }

    return 2*ans;
}

int solve(std::vector<activity> A, std::vector<activity> B) {
    return std::max(solve_one(A,B), solve_one(B,A));
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int Na, Nb;
        std::cin >> Na >> Nb;
        std::vector<activity> A(Na);
        std::vector<activity> B(Nb);
        for (int i = 0; i < Na; i++) {
            std::cin >> A[i].start >> A[i].end;
        }
        for (int j = 0; j < Nb; j++) {
            std::cin >> B[j].start >> B[j].end;
        }
        int ans = solve(A, B);
        std::cout << "Case #" << t << ": " << ans << std::endl;
    }

    return 0;
}

