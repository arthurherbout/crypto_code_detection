#include <iostream>
#include <vector>

int solve3(int a, int b, int c) {
    int ans = a;
    int smaller = std::min(b, c);
    int larger = std::max(b, c);
    ans += smaller;
    ans += (larger-smaller+2)/3;
    return ans;
}

int solve4(int a, int b, int c, int d) {

    int ans = a;

    ans += c/2;
    c %= 2;

    int smaller = std::min(b, d);
    int larger = std::max(b, d);
    ans += smaller;
    b -= smaller;
    d -= smaller;
    int leftover = larger-smaller;

    if (c == 1) {
        ans++;
        c = 0;
        leftover -= 2;
        if (leftover < 0) {
            leftover = 0;
        }
    }
    ans += (leftover+3)/4;
    
    return ans;
}

int solve(std::vector<int> group_size, int p) {

    int n = group_size.size();
    int ans = 0;

    std::vector<int> groups(4);
    for (int i = 0; i < n; i++) {
        groups[group_size[i]%p]++;
    }

    switch (p) {
        case 2:
            ans = groups[0] + (groups[1]+1)/2;
            break;
        case 3:
            ans = solve3(groups[0], groups[1], groups[2]);
            break;
        case 4:
            ans = solve4(groups[0], groups[1], groups[2], groups[3]);
            break;
    }
    
    return ans;
}

int main() {

    int T;
    std::cin >> T;
    for (int t = 1; t <= T; t++) {
        int N;
        int P;
        std::cin >> N >> P;
        std::vector<int> group_size(N);
        for (int i = 0; i < N; i++) {
            std::cin >> group_size[i];
        }
        std::cout << "Case #" << t << ": " << solve(group_size, P) << std::endl;
    }

    return 0;
}

