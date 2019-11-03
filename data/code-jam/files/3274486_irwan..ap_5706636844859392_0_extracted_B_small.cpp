#include <iostream>
#include <vector>

struct activity {
    int start;
    int end;
};

int solve(std::vector<activity> A, std::vector<activity> B) {

    int Na = A.size();
    int Nb = B.size();
    
    if (Na + Nb == 1) {
        return 2;
    }
    
    if (Na == 1 && Nb == 1) {
        return 2;
    }

    if (Na + Nb != 2) {
        return -1;
    }

    std::vector<activity> v;
    if (Na > 0) {
        v = A;
    } else {
        v = B;
    }

    if (v[1].start < v[0].start) {
        std::swap(v[0], v[1]);
    }
    if (v[1].end - v[0].start <= 720) {
        return 2;
    }
    if (v[0].end + 24*60 - v[1].start <= 720) {
        return 2;
    }
    return 4;
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

