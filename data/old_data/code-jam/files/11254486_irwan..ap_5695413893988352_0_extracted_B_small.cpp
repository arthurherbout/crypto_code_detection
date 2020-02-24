#include <iostream>
#include <string>
#include <utility>

std::string toString(int n, int d) {
    std::string ans;
    for (int i = 0; i < d; i++) {
        char c = '0' + n%10;
        ans = c + ans;
        n /= 10;
    }
    return ans;
}

bool match(std::string a, std::string b) {
    int n = a.length();
    for (int i = 0; i < n; i++) {
        if (a[i] == '?' || b[i] == '?') {
            continue;
        }
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

std::pair<int,int> solve(std::string C, std::string J) {

    int l = C.length();
    int limits[4] = {1, 10, 100, 1000};

    int bestDiff = 9999;
    int bestC = -1;
    int bestJ = -1;

    for (int i = 0; i < limits[l]; i++) {
        std::string is = toString(i, l);
        if (!match(is, C)) {
            continue;
        }
        for (int j = 0; j < limits[l]; j++) {
            std::string js = toString(j, l);
            if (!match(js, J)) {
                continue;
            }
            int diff = i - j;
            if (diff < 0) {
                diff = -diff;
            }
            if (diff < bestDiff) {
                bestDiff = diff;
                bestC = i;
                bestJ = j;
            } else if (diff == bestDiff) {
                if (i < bestC) {
                    bestC = i;
                    bestJ = j;
                } else if (i == bestC) {
                    if (j < bestJ) {
                        bestJ = j;
                    }
                }
            }
        }
    }
    return std::pair<int,int>(bestC, bestJ);
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        std::string c, j;
        std::cin >> c >> j;
        int l = c.length();
        std::cout << "Case #" << t << ":";
        std::pair<int,int> ans = solve(c, j);
        std::cout << " " << toString(ans.first, l);
        std::cout << " " << toString(ans.second, l);
        std::cout << std::endl;
    }

    return 0;
}

