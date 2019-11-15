#include <cstdio>
using namespace std;

int main() {

    int T;
    scanf("%d", &T);
    for (int t = 1; t <= T; t++) {
        
        double C, F, X;
        scanf("%lf %lf %lf", &C, &F, &X);

        double rate = 2.0;
        double ans = 0.0;

        while (true) {
            double time_if_wait = X/rate;
            double time_if_buy_factory = C/rate + X/(rate+F);
            if (time_if_buy_factory < time_if_wait) {
                ans += C/rate;
                rate += F;
            } else {
                ans += time_if_wait;
                break;
            }
        }
        printf("Case #%d: %.8lf\n", t, ans);
    }

    return 0;
}

