#include <cstdlib> // EXIT_SUCCESS

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

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

struct IsIn {
    char C;
    IsIn(char c) : C(c) { }
    bool operator() (std::string const & S) {
        return S.find_first_of(C) != std::string::npos;
    }
};
bool MayBe(char C, std::vector<std::string> const & D) {
    std::vector<std::string>::const_iterator it = std::find_if(D.begin(), D.end(), IsIn(C));
    return it != D.end();
}

bool Matches(std::string const & S, std::string const & R) {
    if (S.length() != R.length()) {
        return false;
    }
    for (int I = 0 ; I < S.length() ; ++I) {
        if (R[I] != '_' && R[I] != S[I]) {
            return false;
        }
    }
    return true;
}

void Prune(std::vector<std::string> & D, std::string const & R) {
    for (int I = 0 ; I < D.size() ; ++I) {
        if (!Matches(D[I], R)) {
            D.erase(D.begin() + I);
            --I;
        }
    }
}

void AntiPrune(std::vector<std::string> & D, char C) {
    for (int I = 0 ; I < D.size() ; ++I) {
        if (D[I].find_first_of(C) != std::string::npos) {
            D.erase(D.begin() + I);
        }
    }
}

int Note(std::vector<std::string> D, std::string const & W, std::string const & L) {
    int Score = 0;
    std::string R(W.length(), '_');
    Prune(D, R);
    for (int I = 0 ; I < L.length() ; ++I) {
        char C = L[I];
        if (MayBe(C, D)) {
            if (W.find_first_of(C) == std::string::npos) {
                ++Score;
                AntiPrune(D, C);
            } else {
                for (int J = 0 ; J < W.length() ; ++J) {
                    if (W[J] == C) {
                        R[J] = C;
                    }
                }
                Prune(D, R);
            }
        }
    }
    return Score;
}

std::string TestCase() {
    int N = readInt();
    int M = readInt();
    std::vector<std::string> Di, Li;
    Di.reserve(N); Li.reserve(M);
    while (N > 0) {
        --N;
        Di.push_back(readLine());
    }
    while (M > 0) {
        --M;
        Li.push_back(readLine());
    }
    std::ostringstream Ret;
    for (int I = 0 ; I < Li.size() ; ++I) {
        int Best, Score = -1;
        for (int J = 0 ; J < Di.size() ; ++J) {
            int T = Note(Di, Di[J], Li[I]);
            if (T > Score) {
                Score = T;
                Best = J;
            }
        }
        Ret << Di[Best];
        if (I != Li.size() - 1) {
            Ret << " ";
        }
    }
    return Ret.str();
}

int main(int, char**)
{
    int N = readInt();
    for (int I = 1 ; I <= N ; ++I) {
        std::cout << "Case #" << I << ": " << TestCase() << std::endl;
    }
    return EXIT_SUCCESS;
}

