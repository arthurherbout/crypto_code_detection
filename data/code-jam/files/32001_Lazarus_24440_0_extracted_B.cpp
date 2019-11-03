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
        long int N, M, A;
        std::cin >> N >> M >> A;

        for(long int x1 = 0; x1 <= N; ++x1)
            for(long int x2 = 0; x2 <= N; ++x2)
                for(long int y1 = 0; y1 <= M; ++y1)
                    for(long int y2 = 0; y2 <= M; ++y2)
                    {
                        long int area = labs(x1 * y2 - x2 * y1);
//                        std::cout << "Area of " << x1 << " " << y1 << " " << x2 << " " << y2 <<
//                            " is " << area << std::endl;
                        if(area == A) {
                            std::cout << "Case #" << i << ": 0 0 " <<
                                x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
                            goto nextcase;
                        }
                    }

        std::cout << "Case #" << i << ": IMPOSSIBLE" << std::endl;
nextcase:
        continue;
    }
}
