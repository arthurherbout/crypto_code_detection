#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

int solve (int a, int b) // is winning?
{
    if (a<b) return solve (b, a);
    if (a==b) return 0;
    if (a>=2*b) return 1;
    return 1-solve (b, a%b);
}


long long sol;

int main ()
{
    //freopen ("input.txt", "r", stdin);
    freopen ("C-small-attempt1.in", "r", stdin);
    freopen ("output.txt", "w", stdout);

    int T, A1, A2, B1, B2;
    scanf ("%d", &T);
    for (int t=1; t<=T; t++)
    {
        scanf ("%d %d %d %d", &A1, &A2, &B1, &B2);
        sol = 0;
        for (int a = A1; a<=A2; a++) for (int b = B1; b<=B2; b++) sol += solve (a, b);
        /*for (int a = A1; a<=A2; a++)
        {
            sol += max (B2 - max (B1, 2*a - 1), 0);
            for (int b = min (B1, 2*a); b<min(B2+1, 2*a); b++)
            {
                sol += solve (a,b);
            }
        }
        */
        printf ("Case #%d: %lld\n", t, sol);
    }
    return 0;
}
