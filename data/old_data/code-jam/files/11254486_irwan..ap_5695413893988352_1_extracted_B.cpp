#include <algorithm>
#include <iostream>
#include <string>

std::string fill(std::string C, char d) {
    int n = C.length();
    for (int i = 0; i < n; i++) {
        if (C[i] == '?') {
            C[i] = d;
        }
    }
    return C;
}

std::string difference(std::string a, std::string b) {
    int n = a.length();
    if (a < b) {
        std::swap(a, b);
    }
    std::string ans(n, '0');
    for (int i = n-1; i >=0; i--) {
        int va = a[i]-'0';
        int vb = b[i]-'0';
        if (va < vb) {
            va += 10;
            a[i-1]--;
        }
        int vc = va-vb;
        ans[i] = '0' + vc;
    }
    return ans;
}

std::pair<std::string,std::string> update(std::pair<std::string,std::string> ans, std::string C, std::string J) {
    std::string diff = difference(ans.first, ans.second);
    std::string newDiff = difference(C, J);
    if (newDiff < diff) {
        return std::pair<std::string,std::string>(C, J);
    } else if (newDiff == diff) {
        if (C < ans.first) {
            return std::pair<std::string,std::string>(C, J);
        } else if (C == ans.first && J < ans.second) {
            return std::pair<std::string,std::string>(C, J);
        }
    }
    return ans;
}

std::pair<std::string,std::string> solve(std::string C, std::string J) {

    if (C == "") {
        return std::pair<std::string,std::string>("", "");
    }

    // Both largest digit is revealed
    if (C[0] != '?' && J[0] != '?') {
        if (C[0] > J[0]) {
            C = fill(C, '0');
            J = fill(J, '9');
            return std::pair<std::string,std::string>(C, J);
        } else if (C[0] < J[0]) {
            C = fill(C, '9');
            J = fill(J, '0');
            return std::pair<std::string,std::string>(C, J);
        } else {
            std::string C1 = C.substr(1);
            std::string J1 = J.substr(1);
            std::pair<std::string,std::string> ans1 = solve(C.substr(1), J.substr(1));
            return std::pair<std::string,std::string>(C[0]+ans1.first, J[0]+ans1.second);
        }
    }

    // Both largest digit is hidden
    if (C[0] == '?' && J[0] == '?') {
        // 10
        std::string C1 = C;
        std::string J1 = J;
        C1[0] = '1';
        C1 = fill(C1, '0');
        J1[0] = '0';
        J1 = fill(J1, '9');
        std::pair<std::string,std::string> ans(C1, J1);
        // 01
        std::string C2 = C;
        std::string J2 = J;
        C2[0] = '0';
        C2 = fill(C2, '9');
        J2[0] = '1';
        J2 = fill(J2, '0');
        ans = update(ans, C2, J2);
        // 00
        std::pair<std::string,std::string> ans3 = solve(C.substr(1), J.substr(1));
        std::string C3 = '0' + ans3.first;
        std::string J3 = '0' + ans3.second;
        ans = update(ans, C3, J3);
        return ans;
    }

    // C largest digit is hidden
    if (C[0] == '?') {
        // same
        std::pair<std::string,std::string> ans1 = solve(C.substr(1), J.substr(1));
        std::string C1 = J[0] + ans1.first;
        std::string J1 = J[0] + ans1.second;
        std::pair<std::string,std::string> ans(C1, J1);
        // larger
        if (J[0] != '9') {
            std::string C2 = (char)(J[0]+1) + fill(C.substr(1), '0');
            std::string J2 = fill(J, '9');
            ans = update(ans, C2, J2);
        }
        // smaller
        if (J[0] != '0') {
            std::string C3 = (char)(J[0]-1) + fill(C.substr(1), '9');
            std::string J3 = fill(J, '0');
            ans = update(ans, C3, J3);
        }
        return ans;
    }

    // J largest digit is hidden
    if (J[0] == '?') {
        // same
        std::pair<std::string,std::string> ans1 = solve(C.substr(1), J.substr(1));
        std::string C1 = C[0] + ans1.first;
        std::string J1 = C[0] + ans1.second;
        std::pair<std::string,std::string> ans(C1, J1);
        // larger
        if (C[0] != '9') {
            std::string J2 = (char)(C[0]+1) + fill(J.substr(1), '0');
            std::string C2 = fill(C, '9');
            ans = update(ans, C2, J2);
        }
        // smaller
        if (C[0] != '0') {
            std::string J3 = (char)(C[0]-1) + fill(J.substr(1), '9');
            std::string C3 = fill(C, '0');
            ans = update(ans, C3, J3);
        }
        return ans;
    }

    // impossible
    return std::pair<std::string,std::string>("", "");
}

int main() {

    int T;
    std::cin >> T;
    for (int t = 1; t <= T; t++) {
        std::string C, J;
        std::cin >> C >> J;
        std::pair<std::string,std::string> ans = solve(C, J);
        std::cout << "Case #" << t << ": " << ans.first << " " << ans.second << std::endl;
    }

    return 0;
}

