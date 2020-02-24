#include <stdio.h>
#include <algorithm>

using namespace std;

int cont[2600];

int main() {
    int t, n, x;
    scanf("%d", &t);

    for (int tc = 0; tc < t; tc++) {
        printf("Case #%d:", tc+1);
        scanf("%d", &n);

        for (int i = 0; i < 2600; i++)
            cont[i] = 0;

        for (int i = 0; i < 2*n-1; i++)
            for (int j = 0; j < n; j++) {
                scanf("%d", &x);
                cont[x]++;
            }

        vector < int > used;

        for (int i = 0; i < 2600; i++)
            if (cont[i] % 2 != 0)
                used.push_back(i);

        sort(used.begin(), used.end());

        for (int i = 0; i < used.size(); i++)
            printf(" %d", used[i]);
        printf("\n");
    }
    return 0;
}
