#include <iostream>
#include <set>
#include <vector>
using namespace std;

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int n;
        cin >> n;
        int capacity;
        cin >> capacity;
        multiset<int> sizes;
        for (int i = 0; i < n; i++) {
            int a;
            cin >> a;
            sizes.insert(a);
        }

        int ans = 0;
        while (sizes.size() > 0) {
            ans++;
            multiset<int>::iterator end = sizes.end();
            end--;
            int largest = *end;
            sizes.erase(end);

            if (sizes.size() > 0) {
                multiset<int>::iterator it = sizes.upper_bound(capacity-largest);
                if (it != sizes.begin()) {
                    it--;
                    sizes.erase(it);
                }
            }
        }
        cout << "Case #" << t << ": " << ans << endl;
    }

    return 0;
}

