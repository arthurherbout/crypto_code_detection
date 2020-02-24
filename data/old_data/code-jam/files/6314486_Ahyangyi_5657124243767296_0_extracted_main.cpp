// Simple template
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void insert_edge(vector<pair<int,int>>& edges, vector<vector<int>>& right, int l, int r)
{
    int e = edges.size();
    edges.push_back(make_pair(l, r));
    right[r].push_back(e);
}

bool match(const vector<pair<int,int>>& edges, const vector<vector<int>>& right,
        vector<int>& matched, vector<char>& visited, int i)
{
    visited[i] = 1;
    for (size_t j = 0; j < right[i].size(); ++j)
    {
        int e = right[i][j];
        if (matched[edges[e].first] == -1)
        {
            matched[edges[e].first] = i;
            return true;
        }
    }
    for (size_t j = 0; j < right[i].size(); ++j)
    {
        int e = right[i][j];
        if (!visited[matched[edges[e].first]] && match(edges, right, matched, visited, matched[edges[e].first]))
        {
            matched[edges[e].first] = i;
            return true;
        }
    }

    return false;
}

bool solve(const vector<vector<int>>& dice, int start, int length)
{
    vector<pair<int,int>> edges;
    vector<vector<int>> right(length);

    for (int i = 0; i < dice.size(); ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (dice[i][j] >= start && dice[i][j] < start + length)
            {
                insert_edge(edges, right, i, dice[i][j] - start);
            }
        }
    }

    vector<int> matched(dice.size(), -1);
    for (int i = 0; i < length; ++i)
    {
        vector<char> visited(length);
        if (!match(edges, right, matched, visited, i))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    int t;

    cin >> t;
    for (int ct = 0; ct < t; ++ct)
    {
        vector<vector<int>> dice;
        vector<int> nums;
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
        {
            vector<int> die;
            for (int j = 0; j < 6; ++j)
            {
                int t;
                cin >> t;
                die.push_back(t);
                nums.push_back(t);
            }
            dice.push_back(die);
        }
        sort(nums.begin(), nums.end());
        auto ptr = unique(nums.begin(), nums.end());
        nums.erase(ptr, nums.end());
        
        int best = 1;

        for (size_t i = 0; best < n && i + best < nums.size(); ++i)
        {
            while (best < n && i + best < nums.size() && nums[i + best] == nums[i] + best && solve(dice, nums[i], best + 1))
            {
                ++best;
            }
        }

        cout << "Case #" << (ct + 1) << ": " << best << endl;
    }

    return 0;
}
