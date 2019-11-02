#include<bits/stdc++.h>

using namespace std;

int N, pfMax;
char sir1[30], sir2[30];
long long mini, A, B, INF = 1e18 + 1000;

bool match (int i, int j, int k)
{
    if (sir1[i] != '?' && sir1[i] - '0' != j) return 0;
    if (sir2[i] != '?' && sir2[i] - '0' != k) return 0;
    return 1;
}

long long GetVals (int pf)
{
    long long x = 0;
    for (int i=1; i<=pf; i++)
    {
        int dig;
        if (sir1[i] == '?' && sir2[i] == '?') dig = 0;
        else
        if (sir1[i] == '?') dig = sir2[i] - '0';
        else dig = sir1[i] - '0';
        x = x * 10LL + dig;
    }
    return x;
}

void Update (long long X, long long Y)
{
    long long dif = X - Y;
    if (dif < 0) dif = -dif;
    if (dif < mini) mini = dif, A = X, B = Y;
    else
    if (dif == mini && X < A) A = X, B = Y;
    else
    if (dif == mini && X == A && Y < B) B = Y;
}

void Print (long long val)
{
    int i = N, v[20];
    while (i) v[i] = val % 10, val /= 10, i --;
    for (int i=1; i<=N; i++)
        printf ("%d", v[i]);
}

int main ()
{
freopen ("input", "r", stdin);
freopen ("output", "w", stdout);

int Tests;
scanf ("%d", &Tests);
for (int test_id = 1; test_id <= Tests; test_id ++)
{
    printf ("Case #%d: ", test_id);
    scanf ("\n%s %s", sir1 + 1, sir2 + 1), N = strlen (sir1 + 1), pfMax = N;
    for (int i=1; i<=N; i++)
        if (sir1[i] != '?' && sir2[i] != '?' && sir1[i] != sir2[i])
        {
            pfMax = i - 1;
            break;
        }
    if (pfMax == N)
    {
        long long x = GetVals (N);
        Print (x), printf (" "), Print (x), printf ("\n");
        continue;
    }
    mini = INF;
    for (int pf = 0; pf <= pfMax; pf ++)
    {
        long long curr = GetVals (pf);
        for (int j=0; j<10; j++)
            for (int k=0; k<10; k++)
                if (match (pf + 1, j, k))
                {
                    long long x = curr * 10LL + j, y = curr * 10LL + k;
                    if (j < k)
                    {
                        for (int i=pf + 2; i<=N; i++)
                        {
                            int mini = 20, how1, how2;
                            for (int j=0; j<10; j++)
                                for (int k=0; k<10; k++)
                                    if (match (i, j, k) && k - j < mini) mini = k - j, how1 = j, how2 = k;
                            x = x * 10LL + how1, y = y * 10LL + how2;
                        }
                    }
                    else
                    {
                        for (int i=pf + 2; i<=N; i++)
                        {
                            int mini = 20, how1, how2;
                            for (int j=0; j<10; j++)
                                for (int k=0; k<10; k++)
                                    if (match (i, j, k) && j - k < mini) mini = j - k, how1 = j, how2 = k;
                            x = x * 10LL + how1, y = y * 10LL + how2;
                        }
                    }
                    Update (x, y);
                }
    }
    Print (A), printf (" "), Print (B), printf ("\n");
}

return 0;
}
