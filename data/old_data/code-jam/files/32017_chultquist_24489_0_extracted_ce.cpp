#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

int main()
{
    int t;
    cin >> t;
    for (int c = 1; c <= t; c++)
    {
        int k;
        cin >> k;
        int deck[k];
        memset(deck, 0, sizeof(deck));
        deck[0] = 1;
        int cur = 0;
        vector<int> free;
        for (int i = 1; i < k; i++)
            free.push_back(i);
        for (int i = 2; i <= k; i++)
        {
            cur = (cur + i - 1) % free.size();
            deck[free[cur]] = i;
            free.erase(free.begin() + cur);
        }

        int n;
        cin >> n;
        cout << "Case #" << c << ":";
        for (int i = 0; i < n; i++)
        {
            int d;
            cin >> d;
            cout << " " << deck[d - 1];
        }
        cout << endl;
    }

    return 0;
}
