#include <iostream>
#include <deque>
#include <string>
#include <vector>

using namespace std;

bool sol(vector<deque<pair<int,int>>> decks)
{
    while(true)
    {
        bool changed = false;
        for (int i = 0; i < decks.size(); ++i)
            if (decks[i].size() > 0)
        for (int j = i + 1; j < decks.size(); ++j)
            if (decks[j].size() > 0)
        {
            if (decks[i][0].second == decks[j][0].second)
            {
                changed = true;
                if (decks[i][0].first < decks[j][0].first)
                {
                    decks[i].pop_front();
                }
                else
                {
                    decks[j].pop_front();
                }
            }
        }
        if (!changed)
        {
            break;
        }
    }
    {
        bool finished = true;
        for (int i = 0; i < decks.size(); ++i)
            if (decks[i].size() > 1)
                finished = false;
        if (finished)
        {
            return true;
        }
    }
    {
        for (int i = 0; i < decks.size(); ++i)
            if (decks[i].size() == 0)
            {
                for (int j = 0; j < decks.size(); ++j)
                    if (decks[j].size() > 1)
                    {
                        decks[i].push_back(decks[j][0]);
                        decks[j].pop_front();
                        if (sol(decks))
                        {
                            return true;
                        }
                        decks[j].push_front(decks[i][0]);
                        decks[i].pop_back();
                    }

                return false;
            }
    }
    return false;
}

int main()
{
    int p;
    int t;
    vector<deque<pair<int,int>>>decks;

    cin >> p;
    for (int i = 0; i < p; ++i)
    {
        int n;
        cin >> n;
        deque<pair<int,int>> deck;
        for (int j = 0; j < n; ++j)
        {
            int a, b;
            cin >> a >> b;
            deck.push_back(make_pair(a, b));
        }
        decks.push_back(deck);
    }
    cin >> t;
    for (int ct = 0; ct < t; ++ct)
    {
        int n, c;
        vector<deque<pair<int,int>>> cur_decks;
        cin >> n >> c;
        for (int i = 0; i < n; ++i)
        {
            int idx;
            cin >> idx;
            cur_decks.push_back(decks[idx]);
        }
        if (sol(cur_decks))
        {
            cout << "Case #" << (ct + 1) << ": POSSIBLE" << endl;
        }
        else
        {
            cout << "Case #" << (ct + 1) << ": IMPOSSIBLE" << endl;
        }
    }

    return 0;
}
