#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
int table[10000] = {1, 6,15, 28}; 


int ctable[9][9] = {{1, 0}, {2, 1, 0}, {3, 3, 1},
    {4, 6, 4, 1, 0}, 
    {5, 10, 10, 5, 1}, {6, 15, 20, 15, 6, 1},
    {7, 21, 35, 35, 21, 7, 1},
    {8, 28, 56, 70, 56, 28, 8, 1}, 
    {9, 36, 84, 126, 126, 84, 36, 9, 1}};


int main() {
    int numCase = 0;
    FILE* fin = fopen("test.in", "r");
    fscanf(fin, "%d", &numCase);
    
    int i, j, k;
    for(i = 0 ; i < numCase; i ++) {
        int x, y, n;
        fscanf(fin, "%d %d %d", &n, &x, &y);
        for(j = 0; j < 4; j ++) {
            if(table[j] >= n)break;
        }
        if(x < 0) {
            if(y -x < j * 2) {
                printf("Case #%d: 1.0\n", i +1);
                continue;
            }else if(y-x ==j * 2 && table[j] == n){
                printf("Case #%d: 1.0\n", i +1);
                continue;
            }else if(y-x > j * 2){
                printf("Case #%d: 0.0\n", i +1);
                continue;
            }
        }
        if(x >= 0) {
            if(y +x < j * 2) {
                printf("Case #%d: 1.0\n", i +1);
                continue;
            }else if(y+x == j * 2 && table[j] == n){
                printf("Case #%d: 1.0\n", i + 1);
                continue;
            }else if( y+x >j * 2){
                printf("Case #%d: 0.0\n", i +1);
                continue;
            }
        }

        int rest = n - table[j-1];
        if(rest >= 2*j + 1) {
            if(y <= rest - (2*j +1)) {
                printf("Case #%d: 1.0\n", i + 1);
                continue;
            }
            else{
                rest -= 2 *j;
                y -= rest;
                rest = n-table[j-1] - 2* rest;
            }
        }
        int count = 0;
        double prob = pow(0.5, rest);
        for(k =  y + 1; k <= rest ; k ++ ) {
            count += ctable[rest-1][k-1];
        }

        prob *= count;
        printf("Case #%d: %.7f\n", i + 1, prob);

    }
    fclose(fin);
    return 0;
}
