#include <bitset>
#include <iostream>
#include <set>

using namespace std;

#define RCMAX 10

typedef bitset<RCMAX * RCMAX> bitboard;

int r, c;

bool bitboardless(const bitboard& lhs, const bitboard& rhs)
{
    for (int i=0; i<r; i++) for (int j=0; j<c; j++)
    {
        bool a = lhs[i * RCMAX + j], b = rhs[i * RCMAX + j];
        if (a && !b)
            return false;
        else if (!a && b)
            return true;
    }
    return false;
}

int main()
{
    int t;
    
    char maze[RCMAX][RCMAX];
    int cavei[10], cavej[10];
    int ncaves;
    
    bitboard reachfrom;
    set<bitboard, bool (*)(const bitboard&, const bitboard&)>
        tried(bitboardless), untried(bitboardless);
    const char* l;
    
    int i, j;
    
    cin >> t;
    for (int caseno = 1; caseno <= t; caseno++)
    {
        cin >> r >> c;
        ncaves = 0;
        for (i = 0; i < r; i++) for (j = 0; j < c; j++)
        {
            cin >> maze[i][j];
            if (isdigit(maze[i][j]))
            {
                cavei[maze[i][j]-'0'] = i;
                cavej[maze[i][j]-'0'] = j;
                ncaves++;
            }
        }
        
        cout << "Case #" << caseno << ":" << endl;
        
        for (int caveindex = 0; caveindex < ncaves; caveindex++)
        {
            reachfrom.reset();
            tried.clear();
            untried.clear();
            i = cavei[caveindex];
            j = cavej[caveindex];
            while (maze[i][--j] != '#');
            for (j++; maze[i][j] != '#'; j++)
                reachfrom.set(i * RCMAX + j);
            for ( ; i>0; i--)
            {
                for (j=1; j<c; j++)
                {
                    if (maze[i][j] != '#' && reachfrom[(i+1) * RCMAX + j])
                    {
                        while (maze[i][j-1] != '#')
                            j--;
                        for ( ; maze[i][j] != '#'; j++)
                            reachfrom.set(i * RCMAX + j);
                    }
                }
            }
            l = " Unlucky";
            
            untried.insert(reachfrom);
            while (!untried.empty())
            {
                set<bitboard>::iterator iter = untried.begin();
                bitboard before = *iter, after;
                tried.insert(before);
                untried.erase(iter);
                
                // left.
                for (i=0; i<r; i++) for (j=0; j<c; j++)
                {
                    if (before[i * RCMAX + j])
                    {
                        if (maze[i][j-1] == '#')
                            after.set(i * RCMAX + j);
                        else
                            after.set(i * RCMAX + j-1);
                    }
                }
                if (after.count() == 1)
                {
                    l = " Lucky";
                    break;
                }
                if (tried.count(after) == 0 && untried.count(after) == 0)
                    untried.insert(after);
                after.reset();
                
                // right.
                for (i=0; i<r; i++) for (j=0; j<c; j++)
                {
                    if (before[i * RCMAX + j])
                    {
                        if (maze[i][j+1] == '#')
                            after.set(i * RCMAX + j);
                        else
                            after.set(i * RCMAX + j+1);
                    }
                }
                if (after.count() == 1)
                {
                    l = " Lucky";
                    break;
                }
                if (tried.count(after) == 0 && untried.count(after) == 0)
                    untried.insert(after);
                after.reset();
                
                // down.
                bool okay = true;
                for (i=0; i<r && okay; i++) for (j=0; j<c; j++)
                {
                    if (before[i * RCMAX + j])
                    {
                        if (maze[i+1][j] == '#')
                            after.set(i * RCMAX + j);
                        else if (reachfrom[(i+1) * RCMAX + j])
                            after.set((i+1) * RCMAX + j);
                        else
                        {
                            okay = false;
                            break;
                        }
                    }
                }
                if (okay)
                {
                    if (after.count() == 1)
                    {
                        l = " Lucky";
                        break;
                    }
                    if (tried.count(after) == 0 && untried.count(after) == 0)
                        untried.insert(after);
                }
            }
            cout << caveindex << ": " << reachfrom.count() << l << endl;
        }
        
    }
    return 0;
}
