#include <iostream>
#include <cstdio>
#include <string>
using namespace std;
string sent[] = {"X won", "O won", "Draw", "Game has not completed"};
int main()
{
    //freopen("gcj1.in", "r", stdin);
    freopen("gcj1.out", "w", stdout);
    int t;
    cin>>t;
    for (int tp = 1; tp <= t; tp++)
    {
        int d[4][4] = {};
        bool finished = true;
        for (int j = 0; j < 4; j++)
        {
            string rd;
            cin>>rd;
            for (int k = 0; k < 4; k++)
            {
                if (rd[k] == 'X') d[j][k] = 2;
                if (rd[k] == 'O') d[j][k] = 64;
                if (rd[k] == 'T') d[j][k] = 1;
                if (rd[k] == '.') finished = false;
            }
        }

        int answer = -1, t;

        for (int j = 0; j < 4; j++)
        {
            if (t = d[j][0] * d[j][1] * d[j][2] * d[j][3])
            {
                if (t == 8 || t == 16) {answer = 0; break;}
                if (t == 262144 || t == 16777216) {answer = 1; break;}
            }

            if (t = d[0][j] * d[1][j] * d[2][j] * d[3][j])
            {
                if (t == 8 || t == 16) {answer = 0; break;}
                if (t == 262144 || t == 16777216) {answer = 1; break;}
            }
        }

        if (t = d[0][0] * d[1][1] * d[2][2] * d[3][3])
        {
            if (t == 8 || t == 16) {answer = 0;}
            if (t == 262144 || t == 16777216) {answer = 1;}
        }

        if (t = d[0][3] * d[1][2] * d[2][1] * d[3][0])
        {
            if (t == 8 || t == 16) {answer = 0;}
            if (t == 262144 || t == 16777216) {answer = 1;}
        }

        if (answer == -1) answer = finished ? 2 : 3;
        cout<<"Case #"<<tp<<": "<<sent[answer]<<endl;
    }

    fclose(stdout);
}
