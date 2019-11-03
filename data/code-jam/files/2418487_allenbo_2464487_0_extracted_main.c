#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int numCase =  0;
    FILE* fin = fopen("test.in", "r");
    fscanf(fin, "%d", &numCase);

    int i, j, k;
    for(i = 0; i < numCase; i ++ ) {
        int r = 0;
        int t = 0;
        fscanf(fin, "%d %d", &r, &t);
        for(j = 1; 1 ;j ++ ) {
            int n = (2*r -3) * j + 2 * (j + 1) * j;
            if(n > t) {
                printf("Case #%lld: %lld\n", i + 1, j - 1);
                break;
            }
        }
    }
    return 0;
}
