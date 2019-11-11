#include <bits/stdc++.h>

using namespace std;


int main()
{
    int totalLines;
    unsigned long long int D;
    int N;
    unsigned long long int K;
    double S;
    double speed;
    double maxSpeed;


    freopen("input.in", "r", stdin);
    freopen("output.txt", "w", stdout);

    scanf("%d", &totalLines);

    for (int line= 1; line<=totalLines; line++)
    {
        scanf("%lld", &D);
        scanf("%d", &N);

        maxSpeed=0;
        for (int horse= 1; horse<=N; horse++)
        {
            scanf("%lld", &K);
            scanf("%lf", &S);

            if(D-K > 0)
            {
                speed = D / ((D-K)/S);
            }

            if( horse == 1 ||speed <= maxSpeed )
            {
                maxSpeed = speed;
            }
        }
        printf("Case #%d: %lf\r\n", line, maxSpeed);

    }
}
