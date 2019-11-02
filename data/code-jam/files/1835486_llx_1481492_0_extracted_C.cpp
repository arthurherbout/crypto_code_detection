#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

typedef long long lint;

typedef struct food_t {
    int id;
    lint P, S;
};

food_t food[222];

bool cmp_P(const food_t &a, const food_t &b) {
    if (a.P < b.P) return true;
    else if (a.P > b.P) return false;
    return a.S > b.S;
}

bool cmp_S(const food_t &a, const food_t &b) {
    if (a.S < b.S) return true;
    else if (a.S > b.S) return false;
    return a.P < b.P;
}

int main() {
    FILE *fin, *fout;    
    fin = fopen("D:\\TopCoder\\gcj2012\\R3\\C-small-attempt4.in", "r");
    fout = fopen("D:\\TopCoder\\gcj2012\\R3\\C.out", "w");
    int T;
    fscanf(fin, "%d\n", &T);
    for (int ca = 1; ca <= T; ca++) {
        lint M, F;
        lint N;
        fscanf(fin, "%lld %lld %lld", &M, &F, &N);
        for (int i = 0; i < N; ++i) {
            fscanf(fin, "%lld %lld", &food[i].P, &food[i].S);
            ++food[i].S;
        }
        sort(food, food+N, cmp_P);
        lint cost = F;
        lint day = 0LL;
        for (int i = 0; i < N; ++i) {
            if (cost >= M) break;
            if (F + food[0].P < food[i].P) {//should re buy
                break;
            }
            if (food[i].S <= day) {
                continue;
            }
            lint tmp = food[i].P * (food[i].S - day);
            lint d = food[i].S - day;
            if (tmp+cost > M) {
                d = (M-cost) / food[i].P;
                tmp = d * food[i].P;
            }
            cost += tmp;
            day += d;
        }
        //cout << M << " " << cost << " " << day << endl;///
        lint ans = M / cost * day;
        M %= cost;
        cost = F, day = 0LL;
        for (int i = 0; i < N; ++i) {
            if (cost >= M) break;
            if (F + food[0].P < food[i].P) {//should re buy
                break;
            }
            if (food[i].S <= day) {
                continue;
            }
            lint tmp = food[i].P * (food[i].S - day);
            lint d = food[i].S - day;
            if (tmp+cost > M) {
                d = (M-cost) / food[i].P;
                tmp = d * food[i].P;
            }
            cost += tmp;
            day += d;
        }
        //cout << M << " " << cost << " " << day << endl;///
        ans += day;
        
        cout << ans << endl;///
        fprintf(fout, "Case #%d: %lld\n", ca, ans);
    }
    
    fclose(fin);
    fclose(fout);
    return 0;
}

