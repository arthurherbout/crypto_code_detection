#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

char buffer[50];
int test[50];
char temp[50];

bool
check(int n)
{
    int astart = 0, an = 0;
    bool add = true;
    long int total = 0;
    for(int i = 0; i < n; ++i)
    {
        if(test[i]!=0) {
            strncpy(temp, buffer + astart, an);
            temp[an] = '\0';
//            std::cerr << "Substring (" << astart << ", " << an << "): " << temp << std::endl;
            long int partial = ::atol(temp);
            if(add) {
//                std::cerr << "Adding " << partial << std::endl;
                total += partial;
            } else {
//                std::cerr << "Subtracting " << partial << std::endl;
                total -= partial;
            }
            astart = i;
            an = 0;
            add = (test[i]==1);
        }
        ++an;
    }
    strncpy(temp, buffer + astart, an);
    temp[an] = '\0';
//    std::cerr << "Substring (" << astart << ", " << an << "): " << temp << std::endl;
    long int partial = ::atol(temp);
    if(add) {
//        std::cerr << "Adding " << partial << std::endl;
        total += partial;
    } else {
//        std::cerr << "Subtracting " << partial << std::endl;
        total -= partial;
    }

//    std::cerr << "Total: " << total << std::endl;
    return (total%2==0) || (total%3==0) || (total%5==0) || (total%7==0);
}

int
main(int argc, char *argv[])
{
    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        std::string s;
        std::cin >> s;

        int D = s.length();
//        std::vector<int> test(D);
        for(int j = 0; j < D; ++j)
            test[j] = 0;

        strcpy(buffer, s.c_str());
        long int result = 0;
        while(test[0]==0)
        {
            if(check(D))
                ++result;
            for(int j = D - 1; j >= 0; --j)
            {
                test[j] = (test[j] + 1) % 3;
                if(test[j] > 0)
                    break;
            }
        }

        std::cout << "Case #" << i << ": " << result << std::endl;
    }
}
