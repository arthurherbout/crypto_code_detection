#include <iostream>
#include <map>
#include <set>
#include <vector>

int main(int argc, char *argv[])
{
    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        long R, k, N;
        std::cin >> R >> k >> N;

        long g[N];
        for(int j = 0; j < N; ++j)
            std::cin >> g[j];

        long value[N];   // number of people who ride if queue is at point N
        int next[N];    // number of slots the queue moves from point N

        if(N==1) {
            value[0] = g[0];
            next[0] = 0;
        } else {
            for(int j = 0; j < N; ++j)
            {
                long total = g[j];
                int jj;
                for(jj = 1; jj < N; ++jj)
                {
                    int idx = j + jj;
                    while(idx >= N)
                        idx -= N;
                    long test = total + g[idx];
                    if(test > k) {
                        break;
                    }
                    total = test;
                }
                value[j] = total;
                next[j] = jj;
//                std::cerr << "j=" << j << ": value = " << value[j] << ", next = " << next[j] << std::endl;
            }
        }

        int pos = 0;
        long earnings = 0;
        for(int j = 0; j < R; ++j)
        {
            earnings += value[pos];
            pos += next[pos];
            while(pos >= N)
                pos -= N;
        }

        std::cout << "Case #" << i << ": ";
        std::cout << earnings;
        std::cout << std::endl;
    }
}
