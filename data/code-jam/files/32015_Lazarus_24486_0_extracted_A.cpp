#include <iostream>
#include <map>
#include <set>
#include <vector>

int
main(int argc, char *argv[])
{
    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        int P, K, L;
        std::cin >> P >> K >> L;

        std::vector<long int> freqs;
        for(int j = 0; j < L; ++j)
        {
            long int temp;
            std::cin >> temp;
            freqs.push_back(temp);
        }

        long int result = 0;
        long int pos = 0;
        std::sort(freqs.begin(), freqs.end());
        for(int j = L - 1; j >= 0; --j)
        {
//            std::cerr << "Assigning freq " << freqs[j] << " to the " << (pos/K) << " position of key " << (pos%K) << std::endl;
            result += (1 + (pos/K)) * freqs[j];
            ++pos;
        }

        std::cout << "Case #" << i << ": " << result << std::endl;
    }
}
