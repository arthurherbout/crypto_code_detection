#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

bool isUpDown(const vector<int>& v) {
    int largest_position = 0;
    int n = v.size();
    for (int i = 0; i < n; i++) {
        if (v[i] > v[largest_position]) {
            largest_position = i;
        }
    }
    for (int i = largest_position; i > 0; i--) {
        if (v[i-1] > v[i]) {
            return false;
        }
    }
    for (int i = largest_position; i < n-1; i++) {
        if (v[i+1] > v[i]) {
            return false;
        }
    }
    return true;
}

int count_inversion_recursive(vector<int>& v, int left, int right, vector<int>& buffer) {
    if (left >= right) {
        return 0;
    }
    int mid = (left + right) / 2;
    int ans = 0;
    ans += count_inversion_recursive(v, left, mid, buffer);
    ans += count_inversion_recursive(v, mid+1, right, buffer);
    for (int i = left; i <= right; i++) {
        buffer[i] = v[i];
    }
    int leftptr = left;
    int rightptr = mid+1;
    int cptr = left;
    while (leftptr <= mid && rightptr <= right) {
        if (buffer[leftptr] < buffer[rightptr]) {
            v[cptr++] = buffer[leftptr++];
        } else {
            ans += mid + 1 - leftptr;
            v[cptr++] = buffer[rightptr++];
        }
    }
    while (leftptr <= mid) {
        v[cptr++] = buffer[leftptr++];
    }
    while (rightptr <= right) {
        v[cptr++] = buffer[rightptr++];
    }
    return ans;
}

int count_inversion(vector<int> v) {
    vector<int> buffer(v.size());
    return count_inversion_recursive(v, 0, v.size()-1, buffer);
}

int solve(vector<int> a) {

    int n = a.size();
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        p[i] = i;
    }
    int ans = 1000000000;
    do {
        vector<int> v(n);
        for (int i = 0; i < n; i++) {
            v[i] = a[p[i]];
        }
        if (!isUpDown(v)) {
            continue;
        }
        ans = min(ans, count_inversion(p));
    } while (next_permutation(p.begin(), p.end()));
    return ans;
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        cout << "Case #" << t << ": " << solve(a) << endl;
    }

    return 0;
}

