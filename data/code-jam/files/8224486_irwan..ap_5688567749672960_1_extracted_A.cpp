#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

long long int pw10(int n) {
    long long int ans = 1;
    for (int i = 1; i <= n; i++) {
        ans *= 10;
    }
    return ans;
}

int countDigit(long long int N) {
    int ans = 0;
    while (N > 0) {
        N /= 10;
        ans++;
    }
    return ans;
}

pair<long long int, long long int> split(long long int N) {
    int digit = countDigit(N);
    long long int mid = pw10(digit/2 + digit%2);
    long long int upper = N/mid;
    long long int lower = N%mid;
    return pair<long long int, long long int>(upper, lower);
}

bool isPowerOfTen(long long int x) {
    long long int a = 1;
    for (int i = 1; i <= 16; i++) {
        if (a == x) {
            return true;
        }
        a *= 10;
    }
    return false;
}

long long int reverseNumber(long long int x) {
    if (x == 0) {
        return 0;
    }
    vector<int> digits;
    while (x > 0) {
        digits.push_back(x%10);
        x /= 10;
    }
    long long int ans = 0;
    for (size_t i = 0; i < digits.size(); i++) {
        ans = ans*10 + digits[i];
    }
    return ans;
}


long long int solve(long long int N) {

    if (N <= 10) {
        return N;
    }

    long long int ans = 0;

    pair<long long int, long long int> parts = split(N);
    if (parts.second == 0) {
        ans++;
        N--;
        return ans + solve(N);
    }

    if (isPowerOfTen(parts.first)) {
        ans += parts.second + 1;
        N -= parts.second;
        N--;
        return ans + solve(N);
    }

    ans += parts.second;
    N -= parts.second;
    N++;
    N = reverseNumber(N);

    return ans + solve(N);
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        long long int N;
        cin >> N;
        cout << "Case #" << t << ": " << solve(N) << endl;
    }

    return 0;
}

