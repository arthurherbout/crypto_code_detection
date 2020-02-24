/**
 * @author Leo "Ekinox" Gaspard <ekinox1995@gmail.com>
 */

#include <iostream>

/// The result is always the number of elements minus the luckily good-placed, as there are no pairs.
int main(int, char**) {
    int T;
    std::cin >> T; std::cin.ignore();

    for (int I = 1 ; I <= T ; ++I) {
        int N;
        std::cin >> N; std::cin.ignore();

        int M = N;
        for (int J = 0 ; J < M ; ++J) {
            int T;
            std::cin >> T;
            if (T == J + 1) {
                --N;
            }
        }

        std::cout << "Case #" << I << ": " << N << ".000000" << std::endl;
    }

    return 0;
}

