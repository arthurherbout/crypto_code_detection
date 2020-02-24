#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include <set>
#include <map>

using namespace std;

int main()
{
    int n;
    cin >> n;
    for (int caseNum = 1; caseNum <= n; caseNum++)
    {
        int r, c, f;
        cin >> r >> c >> f;

        bool map[c][r];
        for (int i = 0; i < r; i++)
        {
            cin >> ws;
            for (int j = 0; j < c; j++)
            {
                char ch;
                cin >> ch;
                map[j][i] = (ch == '#');
            }
        }

        int bestDigs = -1;

        int curRow = 0;
        int nextRow = 0;
        for (int i = 0; i < c; i++)
        {
            if (map[i][0])
                curRow += (1 << i);
            if (map[i][1])
                nextRow += (1 << i);
        }

        set<pair<pair<int, int>, pair<int, int> > > done;

        priority_queue<pair<pair<int, pair<int, int> >, pair<int, int> >, vector<pair<pair<int, pair<int, int> >, pair<int, int> > >, greater<pair<pair<int, pair<int, int> >, pair<int, int> > > > q;
        q.push(make_pair(make_pair(0, make_pair(curRow, nextRow)), make_pair(0, 0)));
        while (!q.empty())
        {
            int digs = q.top().first.first;
            int cur = q.top().first.second.first;
            int next = q.top().first.second.second;
            int x = q.top().second.first;
            int y = q.top().second.second;

            //cerr << digs << " " << cur << " " << next << " " << x << " " << y << endl;
            q.pop();

            pair<pair<int, int>, pair<int, int> > index = make_pair(make_pair(x, y), make_pair(cur, next));
            typeof(done.begin()) it = done.find(index);
            if (it == done.end())
            {
                done.insert(index);
            }
            else
                continue;

            if (y == r - 1)
            {
                bestDigs = digs;
                break;
            }

            if ((next & (1 << x)) == 0)
            {
                // Falling
                int fallCount = 1;
                int ny = y + 2;
                while (ny < r && !map[x][ny])
                {
                    fallCount++;
                    ny++;
                }

                //cerr << "fall " << fallCount << " " << x << " " << y << " " << ny << endl;

                if (fallCount <= f)
                {
                    int ncur = 0, nnext = 0;
                    if (ny == r)
                    {
                        bestDigs = digs;
                        break;
                    }
                    if (ny - 2 == y)
                        ncur = next;
                    else
                    {
                        for (int i = 0; i < c; i++)
                            if (map[i][ny - 1])
                                ncur += (1 << i);
                    }
                    for (int i = 0; i < c; i++)
                        if (map[i][ny])
                            nnext += (1 << i);

                    q.push(make_pair(make_pair(digs, make_pair(ncur, nnext)), make_pair(x, ny - 1)));
                }
            }
            else
            {
                for (int i = 0; i < 2; i++)
                {
                    int cx = (i == 0) ? -1 : 1;
                    int nx = x + cx;

                    if (nx >= 0 && nx < c && ((cur & (1 << nx)) == 0) && done.find(make_pair(make_pair(nx, y), make_pair(cur, next))) == done.end())
                        q.push(make_pair(make_pair(digs, make_pair(cur, next)), make_pair(nx, y)));

                    if (nx >= 0 && nx < c && ((next & (1 << nx)) != 0) && ((cur & (1 << nx)) == 0))
                    {
                        int nnext = next & ~(1 << nx);
                        if (done.find(make_pair(make_pair(x, y), make_pair(cur, nnext))) == done.end())
                            q.push(make_pair(make_pair(digs + 1, make_pair(cur, nnext)), make_pair(x, y)));
                    }
                }
            }
        }

        cout << "Case #" << caseNum << ": ";
        if (bestDigs == -1)
            cout << "No";
        else
            cout << "Yes " << bestDigs;
        cout << endl;
    }

    return 0;
}
