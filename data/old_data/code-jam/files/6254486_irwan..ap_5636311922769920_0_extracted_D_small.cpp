#include <iostream>

long long int pw(int base, int exp) {
    long long int ans = 1;
    for (int i = 0; i < exp; i++) {
        ans *= base;
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int K, C, S;
        std::cin >> K >> C >> S; // S=K
        std::cout << "Case #" << t << ":";
        long long int chunk_size = pw(K, C-1);
        for (int i = 0; i < S; i++) {
            std::cout << " ";
            std::cout << (1+i*chunk_size);
        }
        std::cout << std::endl;
    }

    return 0;
}

