#include <stdio.h>
#include <algorithm>
using namespace std;
const int N = 1 << 20;
struct item
{
    int result;
    int index;
};
bool operator < (const item& a, const item& b) {
    return a.result < b.result;
}
item result[N];
int input[1000];
bool found = false;

int main() {
    int t, n;
    result[0].result = -1;
    scanf("%d", &t);
    for (int nt = 1; nt <= t; nt++) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &input[i]);
        }

        sort(input, input + n);
        reverse(input, input + n);

        printf("Case #%d:\n", nt);
        for (int i = 1; i < N; i++) {
            int counter = n, temp = i, sum = 0;
            while (counter-- > 0) {
                if (temp % 2 == 1) {
                    sum += input[counter];
                }
                temp /= 2;
            }

            result[i].result = sum;
            result[i].index = i;
        }

        sort(result, result + N);
        found = false;
        for (int i = 2; i < N; i++) {
            if (result[i - 1].result == result[i].result) {
                bool first = true;
                found = true;
                int counter = n, temp = result[i].index;
                while (counter-- > 0) {
                    if (temp % 2 == 1) {
                        if (!first) {
                            printf(" ");
                        }
                        else {
                            first = false;
                        }
                        printf("%d", input[counter]);
                    }
                    temp /= 2;
                }
                puts("");
                first = true;
                counter = n;
                temp = result[i - 1].index;
                while (counter-- > 0) {
                    if (temp % 2 == 1) {
                        if (!first) {
                            printf(" ");
                        }
                        else {
                            first = false;
                        }
                        printf("%d", input[counter]);
                    }
                    temp /= 2;
                }
                puts("");
                break;
            }
        }
        if (!found) {
            puts("Impossible");
        }
    }

    return 0;
}