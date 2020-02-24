#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

void solve(int row, int col, char tiles[51][51])
{
    bool solved = true;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(tiles[i][j] == '#' and tiles[i+1][j] == '#' 
               and tiles[i][j+1] == '#' and tiles[i+1][j+1] == '#') {
                tiles[i][j] = '/';
                tiles[i+1][j] = '\\';
                tiles[i][j+1] = '\\';
                tiles[i+1][j+1] = '/';
            }
        }
    }

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(tiles[i][j] == '#') {
                solved = false;
                break;
            }
        }
    }

    if(solved) {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout << tiles[i][j];
        }
        cout << endl;
    }
    }else{
        cout << "Impossible" << endl;
    }
}

int main()
{
    int cases, n;

    scanf("%d", &cases);
    for(n = 1; n <= cases; n++) {
        int row, col, i, j;
        char tiles[51][51];

        scanf("%d %d", &row, &col);
        scanf("%c");
        memset(tiles, 0, sizeof(tiles));
        for(i = 0; i < row; i++) {
            for(j = 0; j < col; j++) {
                char c;
                scanf("%c", &c);
                tiles[i][j] = c;
            }
            scanf("%c");
        }
        cout << "Case #" << n << ":" << endl;
        solve(row, col, tiles);
    }
    return 0;
}
