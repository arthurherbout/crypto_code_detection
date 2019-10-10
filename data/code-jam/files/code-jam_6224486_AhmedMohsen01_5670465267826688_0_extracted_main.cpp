#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main()
{
    freopen("C-small-attempt1 (4).in", "r", stdin);
    freopen("out.txt", "w", stdout);
    map < pair<char, char>, pair<char, bool> > data;
    data[make_pair('1', '1')] = make_pair('1', 0);
    data[make_pair('1', 'i')] = make_pair('i', 0);
    data[make_pair('1', 'j')] = make_pair('j', 0);
    data[make_pair('1', 'k')] = make_pair('k', 0);

    data[make_pair('i', '1')] = make_pair('i', 0);
    data[make_pair('i', 'i')] = make_pair('1', 1);
    data[make_pair('i', 'j')] = make_pair('k', 0);
    data[make_pair('i', 'k')] = make_pair('j', 1);

    data[make_pair('j', '1')] = make_pair('j', 0);
    data[make_pair('j', 'i')] = make_pair('k', 1);
    data[make_pair('j', 'j')] = make_pair('1', 1);
    data[make_pair('j', 'k')] = make_pair('i', 0);

    data[make_pair('k', '1')] = make_pair('k', 0);
    data[make_pair('k', 'i')] = make_pair('j', 0);
    data[make_pair('k', 'j')] = make_pair('i', 1);
    data[make_pair('k', 'k')] = make_pair('1', 1);

    bool f = 0, s = 0, th = 0;

    int t;
    cin >> t;
    int z = 1;
    while(t--)
    {
        cout << "Case #"<<z<<": ";
        z+= 1;
        int n, k;
        cin >> n >> k;
        string str, temp = "";
        cin >> str;
        if(n == 1)
            cout << "NO" << endl;
        else
        {
            for(int i = 0 ; i < k; i++)
                temp += str;
            char curr = temp[0], sign = '+';
            int i = 1;
            bool f = 0;
            if(curr == 'i')
                f = 1;
            while(i < temp.size() && f == 0)
            {
                //cout << sign << " " << curr << endl;
                pair <char, bool> p = data[make_pair(curr, temp[i])];
                curr = p.first;
                if(p.second == 1)
                {
                    if(sign == '+')
                        sign = '-';
                    else
                        sign = '+';
                }
                //cout << sign << " " << curr << endl;
                if(sign == '+' && curr == 'i')
                    f = 1;
                i += 1;
            }

            if(i>= temp.size() || f == 0)
            {
                cout << "NO" << endl;
            }
            else
            {
                bool sec = 0;
                curr = temp[i];
                sign = '+';
                if(curr == 'j')
                    sec = 1;
                i += 1;
                while(i < temp.size() && sec == 0)
                {
                    pair <char, bool> p = data[make_pair(curr, temp[i])];
                    curr = p.first;
                    if(p.second == 1)
                    {
                        if(sign == '+')
                            sign = '-';
                        else
                            sign = '+';
                    }
                    if(sign == '+' && curr == 'j')
                        sec = 1;
                    //cout << sign << " " << curr << endl;
                    i += 1;
                }
                //cout << i << endl;
                if(i >= temp.size() || sec == 0)
                    cout << "NO" << endl;
                else
                {
                    bool third = 0;
                    curr = temp[i];
                    sign = '+';
                    i += 1;
                    while(i < temp.size())
                    {
                        pair <char, bool> p = data[make_pair(curr, temp[i])];
                        curr = p.first;
                        if(p.second == 1)
                        {
                            if(sign == '+')
                                sign = '-';
                            else
                                sign = '+';
                        }
                        i+= 1;
                    }
                    if(sign == '+' && curr == 'k')
                        cout << "YES" << endl;
                    else
                        cout << "NO"  << endl;
                }

            }


        }

    }



    return 0;
}
