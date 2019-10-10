#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> solve(std::vector<std::string> cake, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (cake[i][j] == '?') {
                continue;
            }
            for (int k = j-1; k >= 0; k--) {
                if (cake[i][k] != '?') {
                    break;
                }
                cake[i][k] = cake[i][j];
            }
            for (int k = j+1; k < c; k++) {
                if (cake[i][k] != '?') {
                    break;
                }
                cake[i][k] = cake[i][j];
            }
        }
    }
    for (int j = 0; j < c; j++) {
        for (int i = 0; i < r; i++) {
            if (cake[i][j] == '?') {
                continue;
            }
            for (int k = i-1; k >= 0; k--) {
                if (cake[k][j] != '?') {
                    break;
                }
                cake[k][j] = cake[i][j];
            }
            for (int k = i+1; k < r; k++) {
                if (cake[k][j] != '?') {
                    break;
                }
                cake[k][j] = cake[i][j];
            }
        }
    }
    return cake;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int r, c;
        std::cin >> r >> c;
        std::vector<std::string> cake(r);
        for (int i = 0; i < r; i++) {
            std::cin >> cake[i];
        }
        cake = solve(cake, r, c);
        std::cout << "Case #" << t << ":" << std::endl;
        for (int i = 0; i < r; i++) {
            std::cout << cake[i] << std::endl;
        }
    }

    return 0;
}

