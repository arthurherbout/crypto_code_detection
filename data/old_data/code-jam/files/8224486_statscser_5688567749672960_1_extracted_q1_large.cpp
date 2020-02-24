#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cmath>
#define forn for(int i = 0; i < n; ++i)

using namespace std;

int steps(vector<int> digits) {
    int count = 0;
    if (digits.size() == 1) {
        return digits[0];
    }
    if (digits[0] == 0) {
        int carry = 1;
        for (int i = 0; i < digits.size() - 1; ++i) {
            if (carry) {
                if (digits[i] == 0) {
                    digits[i] = 9;
                    carry = 1;
                } else {
                    digits[i]--;
                    carry = 0;
                }
            } else break;
        }
        if (carry) {
            if (digits.back() == 1) {
                digits.pop_back();
            } else {
                digits.back()--;
            }
        }
        return steps(digits) + 1;
    }
    int l = ceil((double)digits.size() / 2.0);
    int tmp = 0;
    for (int i = 0; i < l; ++i) {
        tmp += digits[i] * (int)pow(10, i);
    }
    count += tmp - 1;
    tmp = 0;
    for (int i = digits.size() - 1; i >= l; --i) {
        tmp += digits[i] * (int)pow(10, digits.size() - 1 - i);
    }
    if (tmp > 1) {
        count++;
    }
    count += tmp - 1;
    count += 2;
    count += steps(vector<int>(digits.size() - 1, 9));
    return count;
}

int main(int argc, const char * argv[]) {
    ifstream input("input.txt");
    ofstream output("output.txt");
    int t;
    long long n;
    input >>t;
    for (int i = 0; i < t; ++i) {
        input >>n;
        if (n < 20) {
            output <<"Case #" << i+1 <<": " <<n <<endl;
        } else {
            vector<int> digits;
            while (n >= 10) {
                digits.push_back(n % 10);
                n /= 10;
            }
            digits.push_back(n);
            output <<"Case #" << i+1 <<": " <<steps(digits) <<endl;
        }
    }
    output.close();
    input.close();
    return 0;
}
