/**
 * @author Leo "Ekinox" Gaspard <ekinox1995@gmail.com>
 */

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

std::string Result(std::vector<std::string> & Combines, std::vector<std::string> & Opposed, std::string & Elements) {
    //std::cout << "Result" << std::endl;
    char Combinations[26][26] = {0};
    for (int i = 0 ; i < Combines.size() ; ++i) {
        Combinations[Combines[i][0] - 'A'][Combines[i][1] - 'A'] = Combines[i][2];
        Combinations[Combines[i][1] - 'A'][Combines[i][0] - 'A'] = Combines[i][2];
    }

    //std::cout << "A" << std::endl;
    bool Oppositions[26][26]; 
    for (int i = 0 ; i < 26 ; ++i) {
        for (int j = 0 ; j < 26 ; ++j) {
            Oppositions[i][j] = false;
        }
    }
    for (int i = 0 ; i < Opposed.size() ; ++i) {
        Oppositions[Opposed[i][0] - 'A'][Opposed[i][1] - 'A'] = true;
    }
    //std::cout << "B" << std::endl;

    std::vector<char> Res;
    for (std::string::iterator It = Elements.begin() ; It != Elements.end() ; ++It) {
        //std::cout << "..." << std::endl;
        Res.push_back(*It);
        //std::cout << " 1" << std::endl;
        while (Res.size() > 1 && Combinations[Res[Res.size() - 1] - 'A'][Res[Res.size() - 2] - 'A'] != 0) {
            Res[Res.size() - 2] = Combinations[Res[Res.size() - 1] - 'A'][Res[Res.size() - 2] - 'A'];
            Res.pop_back();
        }
        //std::cout << " 2" << std::endl;
        bool Erase = false;
        for (std::size_t i = 0 ; i < Res.size() ; ++i) {
            for (std::size_t j = 0 ; j < Res.size() ; ++j) {
                if (Oppositions[Res[i] - 'A'][Res[j] - 'A']) {
                    Erase = true;
                    break;
                }
            }
        }
        //std::cout << " 3" << std::endl;
        if (Erase) {
            Res.resize(0);
        }
        //std::cout << " 4" << std::endl;
    }

    std::ostringstream oss;
    oss << "[";
    for (std::size_t i = 0 ; int(i) < int(Res.size()) - 1 ; ++i) {
        oss << Res[i] << ", ";
    }
    if (Res.size() >= 1) {
        oss << Res[Res.size() - 1];
    }
    oss << "]";

    return oss.str();
}

int main(int, char**) {
    int T;
    std::cin >> T; std::cin.ignore();

    for (int I = 0 ; I < T ; ++I) {
        //std::cout << "1";
        int C;
        std::cin >> C;
        //std::cout << " : " << C << std::endl;

        //std::cout << "2 : ";
        std::vector<std::string> Combines;
        while (C > 0) {
            --C;
            std::string S;
            std::cin >> S;
            Combines.push_back(S);
            //std::cout << S << " ";
        }
        //std::cout << std::endl;

        //std::cout << "3";
        int D;
        std::cin >> D;
        //std::cout << " : " << D << std::endl;

        //std::cout << "4 : ";
        std::vector<std::string> Opposed;
        while (D > 0) {
            --D;
            std::string S;
            std::cin >> S;
            Opposed.push_back(S);
            //std::cout << S << " ";
        }
        //std::cout << std::endl;

        //std::cout << "5";
        int N;
        std::cin >> N;
        //std::cout << " : " << N << std::endl;

        //std::cout << "6";
        std::string Order;
        std::cin >> Order;
        //std::cout << " : " << Order << std::endl;

        //std::cout << "7" << std::endl;
        std::cout << "Case #" << (I + 1) << ": " << Result(Combines, Opposed, Order) << std::endl;
    }

    return 0;
}

