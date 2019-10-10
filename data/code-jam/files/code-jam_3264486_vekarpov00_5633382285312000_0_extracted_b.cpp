#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool is(int n) {
    int prev = 10;
    while (n > 0) {
        if (n % 10 > prev) return false;
        prev = n % 10;
        n /= 10;
    }
    return true;
}

int main() {
    //ifstream cin("input.in");
    //ofstream cout("output.txt");
    int t;
    int n;
    cin >> t;
    for (int t1 = 0; t1 < t; t1++) {
        cout << "Case #" << t1 + 1 << ": ";
        cin >> n;
        while (true){
            if (is(n)) {
                cout << n << endl;
                break;
            }
            n -= 1;
        }
    }
    return 0;
}
