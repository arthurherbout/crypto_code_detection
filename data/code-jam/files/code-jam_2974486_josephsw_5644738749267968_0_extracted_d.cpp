#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
    int n;
    freopen("D-small-attempt1.in", "r", stdin);
    freopen("D-small-attempt1.out", "w", stdout);

    scanf("%d ", &n);

    for (int i=1; i<=n; ++i) {
        int nblocks;
        scanf("%d ", &nblocks);

        //printf("i = %d, nblock = %d\n", i, nblocks);

        double naomi[1002], ken[1002];

        for (int j=0; j<nblocks; ++j) {
            scanf("%lf ", &naomi[j]);
        }
        for (int j=0; j<nblocks; ++j) {
            scanf("%lf ", &ken[j]);
        }
        sort(naomi, naomi+nblocks);
        sort(ken, ken+nblocks);

        /*printf("Naomi: ");
        for (int j=0; j<nblocks; ++j) {
            printf("%f ", naomi[j]);
        }
        printf("\nKen: ");
        for (int j=0; j<nblocks; ++j) {
            printf("%f ", ken[j]);
        }
        printf("\n");*/

        //deceit war
        int npos=0, kposfront=0, kposback = nblocks-1;
        int dwarscore=0;
        while (npos != nblocks && kposback > -1) {
            if (naomi[npos] < ken[kposfront]) {
                npos++;
                kposback--;
                continue;
            }
            dwarscore += 1;
            npos++;
            kposfront++;

        }
        /*int dwarscore=0;
        for (int j=0; j<nblocks; ++j) {
            if (naomi[j] > ken[j]) {
                dwarscore += 1;
            }
        }*/

        //orig war
        npos=0;
        int kpos=0;
        int owarscore=0;
        while (npos != nblocks && kpos != nblocks) {
            if (naomi[npos] < ken[kpos]) {
                npos++;
                kpos++;
                continue;
            }
            while (naomi[npos] > ken[kpos]) {
                if (kpos == nblocks) break;
                owarscore += 1;
                kpos++;
            }

        }

        printf("Case #%d: %d %d\n", i, dwarscore, owarscore);
    }
}