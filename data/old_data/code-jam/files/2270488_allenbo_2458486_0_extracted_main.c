#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct chest {
    int open;
    int type;
    int size;
    int keys[40];
}chest;


int c[20] = {0};
int cur = 0;

int keyremove(int keys[800], int numKey, int k) {
    int i;
    for(i = k; i < numKey - 1; i ++) {
        keys[i] = keys[i + 1];
    }
    return numKey - 1;
}

int keyextend(int keys[800], int numKey, int akeys[40], int anumKey) {
    int i;
    for(i = 0; i < anumKey; i ++ ) {
        keys[i+numKey] = akeys[i];
    }
    return anumKey + numKey;
}


int keyadd(int keys[800], int numKey, int p, int k) {
    int i;
    for(i = numKey; i > p; i -- ) {
        keys[i] = keys[i-1];
    }
    keys[p] = k;
    return numKey + 1;
}
/*
int openchest(chest cs[20], int numChest, int keys[800], int numKey) {
    int i, j, k;
    int fail[200] = {0};
    for(i = 0 ; i < numChest; i ++ ) {
        if(cs[i].open != 1) break;
    }
    if(i == numChest) return 1; //all chests are open
    if(numKey == 0) return 0;

    for(i = 0; i < numKey; i ++ ) {
        int k = keys[i];
        if(fail[k] == 1) {
            continue;
        }
        int numFit = 0;
        int fit[20] = {0};
        int size = 0;
        int kk;
        for(j = 0; j < numChest; j ++ ) {
            if(cs[j].open == 1) continue;
            if(cs[j].type != k) continue;
            for( kk = 0; kk < numFit; kk ++ ) {
                if(cs[j].size > cs[fit[kk]].size)
                    break;
            }
            int m = numFit;
            for(; m > kk; m -- ) {
                fit[m] = fit[m-1];
            }
            fit[m] = j;
            numFit ++;
        }

        for(j = 0; j < numFit; j ++ ) {
            int n = fit[j];
            if(cs[n].open == 1) continue;
            if(cs[n].type != k) continue;
            // open cs[j] using k
            cs[n].open = 1;
            c[cur++] = n+1;
            numKey = keyremove(keys, numKey, i);
            if(cs[n].size != 0) {
                numKey = keyextend(keys, numKey, cs[n].keys, cs[n].size);
            }

            int suc = openchest(cs, numChest, keys, numKey);
            if(suc) {
                return 1;
            }
            else {
                cur --;
                cs[j].open = 0;
                numKey = numKey - cs[n].size;
                numKey = keyadd(keys, numKey, i, k);
            }
        }
        fail[k] = 1;
    }
    return 0;
}
*/

int openchest(chest cs[20], int numChest, int keys[800], int numKey, int even) {
    int i, j, k;
    for(i = 0 ; i < numChest; i ++ ) {
        if(cs[i].open != 1) break;
    }
    if(i == numChest) return 1; //all chests are open
    if(numKey == 0) return 0;

    for(i = 0; i < numChest; i ++ ) {
        if(cs[i].open == 1) continue;
        if(even == 1)  {
            int skip = 0;
            if(cs[i].size == 0){
                for(j = 0; j < numChest; j ++ ) {
                    if(cs[i].type == cs[j].type && cs[j].size != 0 && cs[j].open == 0) {
                        skip = 1;
                        break;
                    }
                }
                if(skip) continue;
            }
        }
        for(j = 0; j < numKey; j ++ ) {
            int k = keys[j];
            if(cs[i].type != k) continue;
            // open cs[j] using k
            cs[i].open = 1;
            //printf("Try to open chest %d\n", i + 1);
            c[cur++] = i+1;
            numKey = keyremove(keys, numKey, j);
            if(cs[i].size != 0) {
                numKey = keyextend(keys, numKey, cs[i].keys, cs[i].size);
            }

            int suc = openchest(cs, numChest, keys, numKey, even);
            if(suc) {
                return 1;
            }
            else {
                cur --;
                //printf("Fail to open chest %d\n", i + 1);
                cs[i].open = 0;
                numKey = numKey - cs[i].size;
                numKey = keyadd(keys, numKey, j, k);
            }
        }
    }
    return 0;
}

int precheck(int keys[800], int numKey, chest cs[20], int numChest) {
    int needs[200] = {0};
    int holds[200] = {0};
    
    int i;
    for(i = 0 ; i < numKey; i ++ ) {
        holds[keys[i]]++;
    }
    
    for(i = 0; i < numChest; i ++ ) {
        needs[cs[i].type] ++;
        if(cs[i].size) {
            int j;
            for(j = 0; j < cs[i].size; j ++ ) {
                holds[cs[i].keys[j]] ++;
            }
        }
    }
    int even = 1;
    for(i = 0 ; i< 200; i ++ ) {
        if(needs[i] > holds[i]) return 0;
        if(needs[i] < holds[i]) even = 0;

    }
    if(even == 1) return 2;
    return 1;
}
            
 
int main() {
    char* filename = "test.in";
    int numCase =0;
    
    FILE* fin = fopen(filename, "r");
    fscanf(fin, "%d\n", &numCase);

    int i,j,k;
    int keys[800];
    int numKey = 0;
    int numChest = 0;
    chest cs[20];
    for(i = 0; i < numCase; i ++ ) {
        cur = 0;
        fscanf(fin, "%d %d", &numKey, &numChest);
        for(j = 0; j < numKey; j ++ ) {
            fscanf(fin, "%d", &keys[j]);
        }

        for(j = 0; j < numChest; j ++ ) {
            cs[j].open = 0;
            fscanf(fin, "%d %d", &cs[j].type, &cs[j].size);
            if(cs[j].size) {
                for(k = 0; k < cs[j].size; k ++ ) {
                    fscanf(fin, "%d", &cs[j].keys[k]);
                }
            }
        }
        
        int ret = precheck(keys, numKey, cs, numChest); 
        if(ret == 0){
            printf("Case #%d: IMPOSSIBLE\n", i+1);
        }
        else {
            if(ret == 1) {
                ret = openchest(cs, numChest, keys, numKey, 0);
            }else {
                ret = openchest(cs, numChest, keys, numKey, 1);
            }
            if(ret == 1){
                printf("Case #%d: ", i+1);
                for(j = 0 ; j < numChest; j ++ ) {
                    printf("%d ", c[j]);
                }
                printf("\n");
            }
            else {
                printf("Case #%d: IMPOSSIBLE\n", i+1);
            }

        }
        
    }
    return 0;
}
