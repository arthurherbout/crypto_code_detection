#include <iostream>
#include <assert.h>

using namespace std;

// Write a function to compute the fibonacci series.
// i.e 0, 1, 1, 2, 3, 5, 8, 13,

int fib(int i) {
    if (i == 0)
        return 0;
    if (i == 1)
        return 1;
    return fib(i - 2) + fib(i - 1);
}

// Q: What is the time complexity of this algorithm?
// A: O(n^2)

// Write a function to computer fibonacci, and devise a caching
// mechanism, to improve its time complexity

long long int results[1000];

long long int fibCached(int i) {
    if (i == 0)
        return 0;
    if (i == 1)
        return 1;
    if (results[i] != 0)
        return results[i];
    results[i] = fibCached(i - 2) + fibCached(i - 1);
    return results[i];
}

// Q: What is the time complexity of this algorithm?
// A: O(n)

void test9x0() {
    assert(fib(9) == 34);
    assert(fibCached(59) == 956722026041);
    cout << "9.x.0 passed!" << endl;
}
