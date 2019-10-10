#include <iostream>
#include <vector>

std::vector<long long int> solve(int base, int length) {
    
    std::vector<long long int> ans;

    int peek = (base+length-1) / length;
    for (int i = 0; i < peek; i++) {
        long long int tile = 1;
        int d = i*length;
        long long int pw = 1;
        for (int j = 0; j < length; j++) {
            tile += d*pw;
            d = (d+1)%base;
            pw *= base;
        }
        ans.push_back(tile);
    }


    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int base, length, peek;
        std::cin >> base >> length >> peek;

        std::cout << "Case #" << t << ":";
        if (peek*length < base) {
            std::cout << " IMPOSSIBLE";
        } else {
            std::vector<long long int> tiles = solve(base, length);
            int n = tiles.size();
            for (int i = 0; i < n; i++) {
                std::cout << " ";
                std::cout << tiles[i];
            }
        }
        std::cout << std::endl;
    }

    return 0;
}

