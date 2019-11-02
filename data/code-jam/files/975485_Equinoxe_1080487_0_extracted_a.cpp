/**
 * @author Leo "Ekinox" Gaspard <ekinox1995@gmail.com>
 */

#include <iostream>
#include <deque>

int const INFINITE = 100000000;

typedef std::pair<char, int> button;
typedef std::deque<button> cont;

std::pair<int, int> NextNeeded(char C, cont const & V) {
    for (std::size_t P = 0 ; P < V.size() ; ++P) {
        if (V[P].first == C) {
            return std::pair<int, int>(V[P].second, P);
        }
    }
    return std::pair<int, int>(INFINITE, INFINITE);
}

int Time(cont & V) {
    int PosO = 1,
        PosB = 1;

    std::pair<int, int> NextO = NextNeeded('O', V),
                        NextB = NextNeeded('B', V);

    int NextNeededO = NextO.first,
        NextNeededB = NextB.first; 

    bool OBeforeB = (NextO.second < NextB.second);

    bool ODone,
         BDone;

    bool ButtonPushed;

    int NbTurns = 0;

    // 1 second
    while (!V.empty()) {
        ++NbTurns;
        ODone = BDone = ButtonPushed = false;
        // Move O
        if (PosO != NextNeededO) {
            PosO = (PosO > NextNeededO) ? (PosO - 1) : (PosO + 1);
            ODone = true;
        }
        // Move B
        if (PosB != NextNeededB) {
            PosB = (PosB > NextNeededB) ? (PosB - 1) : (PosB + 1);
            BDone = true;
        }
        // Push the O button
        if (!ODone && OBeforeB && NextNeededO == PosO) {
            V.pop_front();
            NextO = NextNeeded('O', V);
            NextNeededO = NextO.first;
            NextB = NextNeeded('B', V);
            NextNeededB = NextB.first;
            OBeforeB = (NextO.second < NextB.second);
            ButtonPushed = true;
        }
        // Push the B button
        if (!BDone && !ButtonPushed && !OBeforeB && NextNeededB == PosB) {
            V.pop_front();
            NextB = NextNeeded('B', V);
            NextNeededB = NextB.first;
            NextO = NextNeeded('O', V);
            NextNeededO = NextO.first;
            OBeforeB = (NextO.second < NextB.second);
        }
    }

    return NbTurns;
}

int main (int, char**) {
    int T;

    std::cin >> T; std::cin.ignore();

    for (int I = 1 ; I <= T ; ++I) {
        int N;
        std::cin >> N;

        cont V(N);
        for (std::size_t J = 0 ; J < N ; ++J) {
            std::cin >> V[J].first >> V[J].second;
        }

        std::cout << "Case #" << I << ": " << Time(V) << std::endl;
    }
    
    return 0;
}

