#include<cstdio>
#include<fstream>
#include<iomanip>
#include<cstring>
#include<string>
#include<map>
#include<vector>
#include<queue>
#include<deque>

using namespace std;

int T, N;
long double V, X, A[109], B[109], AA, BB;

long double eps = 1e-7;

long double mod (long double x)
{
    if (x < 0)
        return -x;
    return x;
}

bool egal (long double a, long double b)
{
    return (mod ((long double)a - b) < eps);
}

int main ()
{
ifstream f("input");
ofstream g("output");

f>>T;
for (int test = 1; test <= T; test ++)
{
    //printf ("Case #%d: ", test);
    g<<"Case #" << test << ": ";
    f>>N>>V>>X;
    AA = (long double) V * X;
    BB = (long double) V;
    for (int i=1; i<=N; i++)
    {
        long double a, b;
        f>>b>>a;
        A[i] = (long double) a * b;
        B[i] = (long double) b;
    }
    if (N == 1)
    {
        if (egal ((long double)AA / A[1], (long double)BB / B[1]))
            g << fixed << setprecision (12) << (long double) (AA / A[1]) << '\n';
        else
            g<<"IMPOSSIBLE\n";
        continue;
    }
    //printf ("\n"); continue;
    long double best = 1000000000.0, x, y;
    bool ok = 0;
    if (egal ((long double)AA / A[1], (long double)BB / B[1]))
        best = (long double)AA / A[1], ok = 1;
    if (egal ((long double)AA / A[2], (long double)BB / B[2]) && (best > (long double)AA / A[2] + eps || ok == 0))
        best = (double)AA / A[2], ok = 1;
    y = (long double) (AA * B[1] - BB * A[1]) / ((long double)A[2] * B[1] - A[1] * B[2]);
    x = (long double) (BB - y * B[2]) / B[1];
    //y = (double) 0.092778156;
    //x = (double) 207221.843687375;
    if (x < -eps || y < -eps) ;
    else
    {
        if (x < y)
            x = y;
        if (x + eps < best || ok == 0)
            best = x, ok = 1;
    }
    if (ok == 0)
        g<<"IMPOSSIBLE\n";
    else
        g << fixed << setprecision (12) << best << '\n';
}

return 0;
}
