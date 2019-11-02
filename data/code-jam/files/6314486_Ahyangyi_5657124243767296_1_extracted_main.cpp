// Simple template
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int N = 1000001;

void insert_edge(vector<pair<int,int>>& edges, vector<vector<int>>& right, int l, int r)
{
    int e = edges.size();
    edges.push_back(make_pair(l, r));
    right[r].push_back(e);
}

bool match(const vector<pair<int,int>>& edges, const vector<vector<int>>& right,
        vector<int>& matched, vector<int>& visited, int i, int v)
{
    visited[i] = v;
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
        if (visited[matched[edges[e].first]] != v && match(edges, right, matched, visited, matched[edges[e].first], v))
        {
            matched[edges[e].first] = i;
            return true;
        }
    }

    return false;
}

bool solve(vector<int>& right_matched, vector<int>& matched, const vector<pair<int,int>>& edges, const vector<vector<int>>& right, const vector<vector<int>>& dice, int start, int length)
{
    cerr << "solve " << start << " " << length << endl;
    for (int i = 0; i < matched.size(); ++i)
        if (matched[i] != -1 && (matched[i] < start || matched[i] >= start + length))
        {
            right_matched[matched[i]] = 0;
            matched[i] = -1;
        }
    vector<int> visited(N, -1);
    for (int i = 0; i < length; ++i)
    {
        if (!right_matched[start + i])
        {
            if (match(edges, right, matched, visited, start + i, i))
            {
                right_matched[start + i] = 1;
            }
            else
            {
                return false;
            }
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

        vector<pair<int,int>> edges;
        vector<vector<int>> right(N);

        for (int i = 0; i < dice.size(); ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                insert_edge(edges, right, i, dice[i][j]);
            }
        }
        
        vector<int> right_matched(N, 0);
        vector<int> matched(dice.size(), -1);
        int best = 1;

        for (size_t i = 0; best < n && i + best < nums.size(); ++i)
        {
            while (best < n && i + best < nums.size() && nums[i + best] == nums[i] + best)
            {
                if (solve(right_matched, matched, edges, right, dice, nums[i], best + 1))
                {
                    ++best;
                }
                else
                {
                    break;
                }
            }
        }

        cout << "Case #" << (ct + 1) << ": " << best << endl;
    }

    return 0;
}
