#include <cstdlib> // EXIT_SUCCESS

#include <iostream>
#include <string>
#include <sstream>

int readInt() {
    int n;
    std::cin >> n;
    std::cin.ignore();
    return n;
}
std::string readStr() {
    std::string s;
    std::cin >> s;
    return s;
}
std::string readLine(char delim = '\n') {
    std::string s;
    std::getline(std::cin, s, delim);
    return s;
}

std::string TestCase() {
    long long N; std::cin >> N; std::cin.ignore();
    int P_D = readInt();
    int P_G = readInt();
    if (P_G == 100 && P_D < 100) {
        return "Broken";
    }
    if (P_G == 0 && P_D > 0) {
        return "Broken";
    }
    int M = static_cast<int>(std::min(N, 100ll));
    bool Broken = true;
    for (int I = 1 ; I <= M ; ++I) {
        if (((I * P_D) % 100) == 0) {
            Broken = false;
        }
    }
    return Broken ? "Broken" : "Possible";
}

int main(int, char**)
{
    int N = readInt();
    for (int I = 1 ; I <= N ; ++I) {
        std::cout << "Case #" << I << ": " << TestCase() << std::endl;
    }
    return EXIT_SUCCESS;
}

