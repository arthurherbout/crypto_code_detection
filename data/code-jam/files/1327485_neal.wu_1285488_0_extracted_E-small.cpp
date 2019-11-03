#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

const int MAX = 25, ITER = 1000;

int A, M, V;
double win_dp [MAX], new_dp [MAX];

inline double win (int x)
{
    if (x <= 0)
        return 0;

    if (x >= V)
        return 1;

    return win_dp [x];
}

void initialize ()
{

}

void initialize_case ()
{
    for (int i = 0; i < MAX; i++)
        win_dp [i] = new_dp [i] = 0;
}

string solve_case ()
{
    initialize_case ();
    ostringstream output;

    scanf ("%d %d %d", &A, &M, &V);

    for (int it = 0; it < ITER; it++)
    {
        for (int a = 1; a < V; a++)
            for (int bet = 1; bet <= min (a, M); bet++)
                for (int times = 1; bet << (times - 1) <= M; times++)
                {
                    int can_win = bet, can_lose = ((1 << times) - 1) * bet;
                    double win_prob = 1 - 1.0 / (1 << times), lose_prob = 1.0 / (1 << times);
                    new_dp [a] = max (new_dp [a], win_prob * win (a + can_win) + lose_prob * win (a - can_lose));
                }

        for (int i = 0; i < MAX; i++)
            win_dp [i] = new_dp [i];
    }

//    for (int i = 0; i <= V; i++)
//        printf ("%d %lf\n", i, win (i));

    for (int i = 0; i < MAX; i++)
        new_dp [i] = 0;

    int most = 0;

    for (int a = 1; a <= V; a++)
        for (int bet = 1; bet <= min (a, M); bet++)
            for (int times = 1; bet << (times - 1) <= M; times++)
            {
                int can_win = bet, can_lose = ((1 << times) - 1) * bet;
                double win_prob = 1 - 1.0 / (1 << times), lose_prob = 1.0 / (1 << times);
                new_dp [a] = win_prob * win (a + can_win) + lose_prob * win (a - can_lose);

                if (a == V && new_dp [a] == win_dp [a])
                    most = max (most, bet);
            }

        output << setprecision (15) << win (A) << ' ' << most;
    return output.str ();
}

int main ()
{
    initialize ();
    int T; scanf ("%d", &T);

    for (int tc = 1; tc <= T; tc++)
        printf ("Case #%d: %s\n", tc, solve_case ().c_str ());

    return 0;
}
