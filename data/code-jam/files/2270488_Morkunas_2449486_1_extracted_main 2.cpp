#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream input("B-large.in");
    int t;
    input >> t;
    int n, m;
    ofstream out("B-large.out");
    bool a, b, c;
    for (int i=0; i<t; i++)
    {
    input >> n >> m;
    int sodas[n][m];
    for (int y=0; y<n; y++)
    {
        for (int z=0; z<m; z++)
        {
            input >> sodas[y][z];
        }
    }
    c=false;
    for (int y=0; y<n; y++)
    {
        for (int z=0; z<m; z++)
        {
        a=false;
            b=false;
        for (int k=0; k<n; k++)
            {

                if (sodas[k][z]>sodas[y][z])
                {
                    a=true;
                }
            }
            for (int k=0; k<m; k++)
            {
                if (sodas[y][k]>sodas[y][z])
                {
                    b=true;
                }
            }
            if ((b == true) && (a == true))
            {
                c=true;
            }
        }
    }
    if (c==false)
    {
        out << "Case #" << i+1 << ": YES" << endl;
    }
    else
    {
        out << "Case #" << i+1 << ": NO" << endl;
    }
    }
    return 0;
}
