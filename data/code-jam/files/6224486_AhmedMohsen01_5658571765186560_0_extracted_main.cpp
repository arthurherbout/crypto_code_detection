#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main()
{
    freopen("D-small-attempt0.in", "r", stdin);
    freopen("out.txt", "w", stdout);
    int x, r, c;
    int t;
    cin >> t;
    int i = 1;
    while(t--)
    {
        cout << "Case #"<<i<<": ";
        i += 1;
        cin >> x >> r >> c;
        bool w = 0;
        if(r == 1)
        {
            if(c == 1)
            {
                if(x == 1)
                    w = 1;
            }
            else if(c == 2)
            {
                if(x == 1 || x == 2)
                    w = 1;
            }
            else if(c == 3)
            {
                if(x == 1 )
                    w = 1;
            }
            else if(c == 4)
            {
                if(x == 1 || x == 2)
                    w = 1;
            }
        }
        else if(r == 2)
        {
            if(c == 1)
            {
                if(x == 1 || x == 2)
                    w = 1;
            }
            else if(c == 2)
            {
                if(x == 1 || x == 2)
                    w = 1;
            }
            else if(c == 3)
            {
                if(x == 1 || x == 2 || x == 3)
                    w = 1;
            }
            else if(c==4)
            {
                if(x == 1 || x==2)
                    w = 1;
            }
        }
        else if(r == 3)
        {
            if(c == 1)
            {
                if(x == 1 )
                    w = 1;
            }
            else if(c == 2)
            {
                if(x == 1 || x == 2 || x == 3)
                    w = 1;
            }
            else if(c == 3)
            {
                if(x == 1 || x == 3)
                    w = 1;
            }
            else if(c == 4)
            {
                if(x == 1 || x == 3  || x == 2 || x == 4)
                    w = 1;
            }
        }
        else if(r == 4)
        {
            if(c == 1)
            {
                if(x == 1 || x == 2)
                    w = 1;
            }
            else if(c == 2)
            {
                if(x == 1 || x==2 )
                    w = 1;
            }
            else if(c==3)
            {
                if(x == 1 || x == 3  || x == 2 || x == 4)
                    w = 1;
            }
            else if( c== 4)
            {
                if(x == 1 || x == 2 || x == 4)
                    w = 1;
            }
        }
        if(w)
            cout << "GABRIEL" << endl;
        else
            cout << "RICHARD" << endl;

    }


    return 0;
}
