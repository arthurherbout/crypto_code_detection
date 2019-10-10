#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<iomanip>
#include<utility>

using namespace std;

class Case{
    int n; string s;
public:
    Case(int nn, string ss = string()) :
        n(nn), s(ss)
    {
    }

    friend ostream& operator<<(ostream& out, Case& c)
    {
        return out << "Case #" << c.n << ": " << c.s ;
    }
};

int main()
{
    ifstream cin("C-small-attempt0.in");
    ofstream cout("a.out");
    int t, n;
    cin >> t;
    for (int i = 1; i <= t; i++)
    {
        int counter = 0;
        cin >> n;
        for (int j = 0; j < n; j++)
        {
            int t;
            cin >> t;
            if (t == j)
            {
                counter++;
            }
        }
        if (counter >= 1)
            cout << Case(i, "BAD\n");
        else
            cout << Case(i, "GOOD\n");
    }

   
    

  //  system("PAUSE");

}