#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iostream>
#include <queue>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef vector<int> vec_t;
typedef vector<vector<int> >  prices_t;

int
optimize(const prices_t& prices, const vec_t& misses, int round, int match)
{
//    cerr << "round = " << round << "; match = " << match << endl;
    if(round == 0) {
        if(misses[2*match] > 0 && misses[2*match+1] > 0)
            return 0;
        else
            return prices[round][match];
    }

    // try not selecting this match
    bool possible_not_to_select = true;
    vec_t misses_new = misses;
    int nrounds = prices.size();
    int nteams = misses.size();
    int k = nrounds - 1 - round;
    int section_size = nteams / (1 << k);

//    cerr << "nrounds = " << nrounds << "; nteams = " << nteams << "; k = " << k << "; section_size = " << section_size << endl;
    for(int i = 0; i < section_size; ++i)
    {
        int idx = i + match * section_size;
//        cerr << idx << endl;
        if(misses_new[idx] == 0) {
            possible_not_to_select = false;
            break;
        }
        --misses_new[idx];
    }

    int total0 = prices[round][match];
    total0 += optimize(prices, misses, round - 1, 2 * match);
    total0 += optimize(prices, misses, round - 1, 2 * match + 1);

    int total1 = 0;
    if(possible_not_to_select) {
        total1 += optimize(prices, misses_new, round - 1, 2 * match);
        total1 += optimize(prices, misses_new, round - 1, 2 * match + 1);
        if(total1 < total0)
            total0 = total1;
    }
    return total0;
}

int main(int argc, char *argv[])
{
    int ncases;
    cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        int P;
        cin >> P;

        int Msize = 1 << P;
        vector<int> M(Msize);
        for(int j = 0; j < Msize; ++j)
            cin >> M[j];

        vector<vector<int> > prices(P);
        for(int j = P-1; j >= 0; --j)
        {
            int round = P-1-j;
            prices[round].resize(1 << j);
            for(int k = 0; k < prices[round].size(); ++k)
                cin >> prices[round][k];
        }
/*
        for(int j = 0; j < P; ++j)
        {
            for(int k = 0; k < prices[j].size(); ++k)
                cout << prices[j][k] << " ";
            cout << endl;
        }
*/
        int best = optimize(prices, M, P-1, 0);

        cout << "Case #" << i << ": ";
        cout << best;
        cout << std::endl;
    }
}
