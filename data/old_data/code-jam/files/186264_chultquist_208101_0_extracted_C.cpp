#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <cassert>
#include <functional>

using namespace std;

#define MID 2000

class node
{
public:
    void clear()
    {
        memset(reached, 0, sizeof(reached));
    }

    bool reached[4000];
    string exprs[4000];

    int digit;
    int operation;
};

node grid[20][20];

int cx[4] = {1, -1, 0, 0};
int cy[4] = {0, 0, 1, -1};

char ops[2] = {'+', '-'};
char nums[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

#define MAX 1800

struct comparator : public binary_function<pair<pair<string, int>, pair<int, int> >, pair<pair<string, int>, pair<int, int> >, bool>
{
    bool operator ()(const pair<pair<string, int>, pair<int, int> > &x, const pair<pair<string, int>, pair<int, int> > &y) const
    {
        if (x.first.first.length() < y.first.first.length())
            return false;
        if (x.first.first.length() > y.first.first.length())
            return true;
        if (x.first.first < y.first.first)
            return false;
        if (x.first.first > y.first.first)
            return true;
        if (abs(x.first.second - MID) < abs(y.first.second - MID))
            return false;
        if (abs(x.first.second - MID) > abs(y.first.second - MID))
            return true;

        return x.second > y.second;
    }
};

int main()
{
    int t;
    cin >> t;

    for (int caseNum = 1; caseNum <= t; caseNum++)
    {
        int w, queries;
        cin >> w >> queries;

        priority_queue<pair<pair<string, int>, pair<int, int> >, vector<pair<pair<string, int>, pair<int, int> > >, comparator > q;

        for (int i = 0; i < w; i++)
        {
            cin >> ws;
            for (int j = 0; j < w; j++)
            {
                char ch;
                cin >> ch;
                grid[i][j].clear();

                if (ch >= '0' && ch <= '9')
                {
                    int dig = ch - '0';
                    grid[i][j].operation = -1;
                    grid[i][j].digit = dig;
                    grid[i][j].reached[MID + dig] = true;
                    grid[i][j].exprs[MID + dig] = string("") + ch;
                    q.push(make_pair(make_pair(grid[i][j].exprs[MID + dig], dig), make_pair(i, j)));
                    //cerr << " " << grid[i][j].digit;
                }
                else
                {
                    if (ch == '+')
                        grid[i][j].operation = 0;
                    else
                    {
                        assert(ch == '-');
                        grid[i][j].operation = 1;
                    }
                    //cerr << " " << -(grid[i][j].operation + 1);
                }
            }
            //cerr << endl;
        }
        //cerr << w << endl;
        cin >> ws;

        while (!q.empty())
        {
            pair<int, int> pos = q.top().second;
            int total = q.top().first.second;
            string exp = q.top().first.first;

            q.pop();

            if (exp != grid[pos.first][pos.second].exprs[MID + total])
                continue;

            //if (pos.first == 2 && pos.second == 2 && total == 40)
            //    cerr << "At " << pos.first << " ; " << pos.second << " with " << total << " and " << exp << endl;

            for (int c = 0; c < 4; c++)
            {
                int ox = pos.first + cx[c];
                int oy = pos.second + cy[c];

                if (ox < 0 || ox >= w || oy < 0 || oy >= w)
                    continue;

                int op = grid[ox][oy].operation;
                for (int c2 = 0; c2 < 4; c2++)
                {
                    int nx = ox + cx[c2];
                    int ny = oy + cy[c2];
                    if (nx < 0 || nx >= w || ny < 0 || ny >= w)
                        continue;

                int ntotal;
                if (op == 0)
                    ntotal = total + grid[nx][ny].digit;
                else
                    ntotal = total - grid[nx][ny].digit;

                if (ntotal > MAX || ntotal < -MAX)
                    continue;

                int index = MID + ntotal;
                string expr = exp + ops[op] + nums[grid[nx][ny].digit];
                if (!grid[nx][ny].reached[index] || (expr.length() < grid[nx][ny].exprs[index].length() || (expr.length() == grid[nx][ny].exprs[index].length() && expr < grid[nx][ny].exprs[index])))
                {
                    grid[nx][ny].reached[index] = true;
                    grid[nx][ny].exprs[index] = expr;

                    q.push(make_pair(make_pair(expr, ntotal), make_pair(nx, ny)));
                }
                }
            }
        }

        cerr << "Solved for case " << caseNum << " of " << t << endl;

        cout << "Case #" << caseNum << ":" << endl;
        for (int k = 0; k < queries; k++)
        {
            int target;
            cin >> target;
            //cerr << " * " << target << endl;
            target += MID;

            bool found = false;
            string expr;
            for (int i = 0; i < w; i++)
                for (int j = 0; j < w; j++)
                    if (grid[i][j].reached[target] && (!found || grid[i][j].exprs[target].length() < expr.length() || (expr.length() == grid[i][j].exprs[target].length() && grid[i][j].exprs[target] < expr)))
                    {
                        expr = grid[i][j].exprs[target];
                        found = true;
                    }

            cout << expr << endl;
        }
    }

    return 0;
}
