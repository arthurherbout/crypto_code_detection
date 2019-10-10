#include <iostream>
#include <map>
#include <set>
#include <vector>

/*
class purity_computator
{
public:
    int table[1000][1000];

    purity_computator()
    {
        for(int i = 0; i < 1000; ++i)
        {
            for(int j = 0; j < 1000; ++j)
            {
                table[i][j] = i==2 ? 1 : -1;
            }
            table[i][1] = 1;
            table[i][2] = 1;
            if(i >= 1)
            table[i][i-1] = 1;
        }
    }

    int num_sets(int n, int size)
    {
        if(size == 1)
            return 1;
        if(n == 2)
            return 1;

    }

    int total_num_sets(int n)
    {
        if(n == 2)
            return 1;

        int total = 0;
        for(int i = 1; i < n; ++i)
            total
    }
}
*/

int main(int argc, char *argv[])
{
    int combin[1000][1000];
    int table[1000][1000];
    int f[1000];
    int mod = 100003;

    combin[0][0] = 1;
    for(int i = 1; i < 1000; ++i)
    {
        combin[i][0] = 1;
        for(int j = 1; j <= i; ++j)
        {
            combin[i][j] = (combin[i-1][j] + combin[i-1][j-1]) % mod;
        }
    }
/*
    for(int i = 0; i < 1000; ++i)
    {
        for(int j = 0; j <= i; ++j)
        {
            std::cout << "C(" << i << "," << j << ") = " << combin[i][j] << std::endl;
        }
    }
*/

    table[2][1] = 1;
    for(int i = 3; i < 1000; ++i)
    {
        table[i][1] = 1;
        table[i][2] = 1;
        for(int j = 3; j < i; ++j)
        {
            int total = 0;
            int gap = i - j - 1;
            for(int k = 1; k < j; ++k)
            {
                if(j-k-1 > gap)
                    continue;
                total = (total + table[j][k] * combin[gap][j-k-1]) % mod;
            }
            table[i][j] = total;
        }
    }

    for(int i = 2; i < 1000; ++i)
    {
        f[i] = 0;
        for(int j = 1; j < i; ++j)
        {
            f[i] = (f[i] + table[i][j]) % mod;
        }
    }

    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        int n;
        std::cin >> n;

        std::cout << "Case #" << i << ": " << f[n];
        std::cout << std::endl;
    }
}
