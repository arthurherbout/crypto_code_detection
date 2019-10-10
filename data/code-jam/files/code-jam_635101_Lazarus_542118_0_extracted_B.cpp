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
        int N, K, B, T;
        std::cin >> N >> K >> B >> T;

        std::vector<int> X, V;
        X.reserve(N);
        V.reserve(N);

        std::vector<bool> can_make_it;
        can_make_it.reserve(N);

        for(int j = N-1; j >= 0; --j)
            std::cin >> X[j];

        for(int j = N-1; j >= 0; --j)
            std::cin >> V[j];

        int number_that_can_make_it = 0;
        for(int j = 0; j < N; ++j)
        {
            can_make_it[j] = (T * V[j] + X[j] >= B);
            if(can_make_it[j])
                ++number_that_can_make_it;
        }

        std::cout << "Case #" << i << ": ";
        if(number_that_can_make_it < K) {
            std::cout << "IMPOSSIBLE";
        } else {
            int num_swaps = 0, pos = 0;
            for(int j = 0; j < N; ++j)
            {
                if(can_make_it[j]) {
                    num_swaps += (j - pos);
                    ++pos;
                    if(pos==K)
                        break;
                }
            }
            std::cout << num_swaps;
        }
        std::cout << std::endl;
    }
}
