#include <cstdio>
#include <algorithm>
#include <cstring>

bool seen[110*110*110*110];
int q[2][110*110*110*110];
int qn[2];
int MMM = 103;

inline int pack(int hd, int ad, int hk, int ak) {
    if (hd < 0) hd = MMM - 1;
    if (hk < 0) hk = MMM - 1;
    return ((hd * MMM + ad) * MMM + hk) * MMM + ak;
}

inline void unpack(int pos, int *hd, int *ad, int *hk, int *ak) {
    *hd = pos / MMM / MMM / MMM;
    *ad = pos / MMM / MMM % MMM;
    *hk = pos / MMM % MMM;
    *ak = pos % MMM;
    if (*hd == MMM - 1) *hd = -1;
    if (*hk == MMM - 1) *hk = -1;
}

int solve(int Hd, int Ad, int Hk, int Ak, int B, int D) {
    memset(seen, 0, sizeof(seen));
    int pos = pack(Hd, Ad, Hk, Ak);
    seen[pos] = true;
    q[0][0] = pos;
    qn[0] = 1;
    for (int d = 0; d < 110; ++d) {
        int dcur = d % 2;
        int dnext = (d + 1) % 2;
        qn[dnext] = 0;
        for (int i = 0; i < qn[dcur]; ++i) {
            int pos = q[dcur][i];
            int hd, ad, hk, ak;
            unpack(pos, &hd, &ad, &hk, &ak);
            if (hk <= 0) {
                return d;
            }

            if (hd <= 0) {
                continue;
            }

            int nextpos;
            // attack
            nextpos = pack(hd - ak, ad, hk - ad, ak);
            if (!seen[nextpos]) { seen[nextpos] = true; q[dnext][qn[dnext]++] = nextpos; }

            // buff
            nextpos = pack(hd - ak, ad + B, hk, ak);
            if (!seen[nextpos]) { seen[nextpos] = true; q[dnext][qn[dnext]++] = nextpos; }

            // cure
            nextpos = pack(Hd - ak, ad, hk, ak);
            if (!seen[nextpos]) { seen[nextpos] = true; q[dnext][qn[dnext]++] = nextpos; }

            // debuff
            nextpos = pack(hd - std::max(0, ak - D), ad, hk, std::max(0, ak - D));
            if (!seen[nextpos]) { seen[nextpos] = true; q[dnext][qn[dnext]++] = nextpos; }
        }
        if (qn[dnext] == 0) {
            return -1;
        }
    }
    return -1;
}

int main() {
    int case_count;
    scanf(" %d", &case_count);
    for (int case_number = 1; case_number <= case_count; ++case_number) {
        int Hd, Ad, Hk, Ak, B, D;
        scanf(" %d %d %d %d %d %d", &Hd, &Ad, &Hk, &Ak, &B, &D);

        int solution = solve(Hd, Ad, Hk, Ak, B, D);
        if (solution == -1) {
            printf("Case #%d: IMPOSSIBLE\n", case_number);
        } else {
            printf("Case #%d: %d\n", case_number, solution);
        }
    }
}
