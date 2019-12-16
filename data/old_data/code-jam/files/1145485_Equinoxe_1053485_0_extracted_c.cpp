#include <cstdlib> // EXIT_SUCCESS

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <deque>
#include <algorithm>

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

struct Card {
    int C, S, T;
};

bool GivesATurn(Card const & C) {
    return C.T > 0;
}
bool MaxScore(Card const & A, Card const & B) {
    return A.S < B.S;
}

std::string TestCase() {
    int N = readInt();
    std::list<Card> Hand;
    for (int I = 0 ; I < N ; ++I) {
        Card C;
        C.C = readInt();
        C.S = readInt();
        C.T = readInt();
        Hand.push_back(C);
    }
    int M = readInt();
    std::deque<Card> Deck;
    for (int I = 0 ; I < M ; ++I) {
        Card C;
        C.C = readInt();
        C.S = readInt();
        C.T = readInt();
        Deck.push_back(C);
    }
    int Score = 0, NbTurns = 1;
    while (NbTurns > 0 && !Hand.empty()) {
        --NbTurns;
        std::list<Card>::iterator it = std::find_if(Hand.begin(), Hand.end(), GivesATurn);
        if (it == Hand.end()) {
            it = std::max_element(Hand.begin(), Hand.end(), MaxScore);
        }
        NbTurns += it->T;
        Score   += it->S;
        while (it->C > 0 && !Deck.empty()) {
            --(it->C);
            Hand.push_back(Deck.front());
            Deck.pop_front();
        }
        Hand.erase(it);
    }
    std::ostringstream Ret;
    Ret << Score;
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

