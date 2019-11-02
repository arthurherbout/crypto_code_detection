#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
const int MAXN = 25 + 5;
string file_name = "A-large";

int gcd(int a, int b)
{
    while(a&&b)
    {
        if(a>b) a%=b;
        else b%=a;
    }
    return a+b;
}

ifstream input(file_name + ".in");
ofstream out(file_name +".out");
//#define input cin
//#define out cout
typedef pair<int,int> pii;
string mas[MAXN];
int R,C;
bool canGo(int y, int x)
{
    if(x>=0 && y >= 0 && x < R && y < C && mas[y][x] == '?')
        return true;
    return false;
}
void fillMasDownUp(int y, int x)
{
    int cy = y;
    int cx = x;
    char letter = mas[y][x];
    cy++;
    while(canGo(cy,cx))
    {
        mas[cy][cx] = letter;
        cy++;
    }
    cy = y;
    cy--;
    while(canGo(cy,cx))
    {
        mas[cy][cx] = letter;
        cy--;
    }

}

void fillMasRightLeft(int y, int x)
{
    int cy = y;
    int cx = x;
    char letter = mas[y][x];
    cx++;
    while(canGo(cy,cx))
    {
        mas[cy][cx] = letter;
        cx++;
    }
    cx = x;
    cx--;
    while(canGo(cy,cx))
    {
        mas[cy][cx] = letter;
        cx--;
    }

}

void solution()
{


    for(int y = 0; y < C; y++)
    {
        for(int x = 0; x < R; x++)
        {
            if(mas[y][x]!= '?')
            fillMasDownUp(y,x);
        }
    }
    for(int y = 0; y < C; y++)
    {
        for(int x = 0; x < R; x++)
        {
            if(mas[y][x]!= '?')
            fillMasRightLeft(y,x);
        }
    }
}





int main()
{
    int T,N;
    input >> T;
    for(int c = 0; c < T; c++)
    {
        input >> C >> R;
        for(int y = 0; y < C; y++)
        {
                input >> mas[y];

        }
        solution();
        out << "Case #" << c+1 << ":" << endl;

        for(int y = 0; y < C; y++)
        {
            for(int x = 0; x < R; x++)
            {
                out << mas[y][x];
            }
            out << endl;
        }
    }
    return 0;
}
