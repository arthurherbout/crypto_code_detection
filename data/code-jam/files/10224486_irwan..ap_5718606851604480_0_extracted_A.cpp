#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string expand_one(std::string s) {
    std::string ans;
    int n = s.length();
    for (int i = 0; i < n; i++) {
        switch (s[i]) {
            case 'R':
                ans += "RS";
                break;
            case 'P':
                ans += "PR";
                break;
            case 'S':
                ans += "PS";
                break;
        }
    }
    return ans;
}

std::string expand(char c, int N) {
    std::string s = std::string("") + c;
    for (int i = 0; i < N; i++) {
        s = expand_one(s);
    }
    return s;
}

bool is_valid(std::string str, int R, int P, int S) {
    int r = 0;
    int p = 0;
    int s = 0;
    int n = str.length();
    for (int i = 0; i < n; i++) {
        switch (str[i]) {
            case 'R':
                r++;
                break;
            case 'P':
                p++;
                break;
            case 'S':
                s++;
                break;
        }
    }
    return r==R && p==P && s==S;
}

std::string sort_n(std::string s, int l) {
    int L = s.length();
    int n = L/l;
    std::string ans;
    for (int i = 0; i < n; i++) {
        std::string a = s.substr(i*l, l/2);
        std::string b = s.substr(i*l+l/2, l/2);
        if (b < a) {
            std::swap(a, b);
        }
        ans += a;
        ans += b;
    }
    return ans;
}

std::string special_sort(std::string s) {
    int l = 2;
    int L = s.length();
    while (l <= L) {
        s = sort_n(s, l);
        l *= 2;
    }
    return s;
}

std::string solve(int N, int R, int P, int S) {
    std::string sp = special_sort(expand('P', N));
    std::string sr = special_sort(expand('R', N));
    std::string ss = special_sort(expand('S', N));
    std::string ans = "Z";
    if (is_valid(sp, R, P, S)) {
        ans = std::min(ans, sp);
    }
    if (is_valid(sr, R, P, S)) {
        ans = std::min(ans, sr);
    }
    if (is_valid(ss, R, P, S)) {
        ans = std::min(ans, ss);
    }
    if (ans == "Z") {
        ans = "IMPOSSIBLE";
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int R, P, S, N;
        std::cin >> N >> R >> P >> S;
        std::string ans = solve(N, R, P, S);
        std::cout << "Case #" << t << ": " << ans << std::endl;
    }

    return 0;
}

