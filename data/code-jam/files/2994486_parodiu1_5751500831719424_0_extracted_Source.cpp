#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<iomanip>
#include<utility>
#include<unordered_set>
#include<unordered_map>

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
    ifstream cin("A-small-attempt0.in");
    ofstream cout("a.out");
    int t, n;
    cin >> t;
    for (int test = 1; test <= t; test++)
    {
        cin >> n;
        vector<string> strs;
        string x;
        getline(cin, x);
        for (int i = 0; i < n; i++) 
        {
            string s;
            getline(cin, s);
            strs.push_back(s);
        }
        string ref(strs[0]);
        ref.resize(distance(ref.begin(), unique(ref.begin(), ref.end())));
        vector<int> minimum, maximum;
        vector<vector<int>> data;
        vector<int> diff;
        for (string s : strs) 
        {
            int i = s.length();
            s.resize(distance(s.begin(), unique(s.begin(), s.end())));
            if (ref != s)
            {
                cout << Case(test, "Fegla Won\n");
                goto endloop;
            }
        }

        {
            for (string &s : strs)
            {
                vector<int> v;
                char prev = '\0';
                int counter = 0;
                for (char c : (s + "?"))
                {
                    if (c != prev) {
                        v.push_back(counter);
                        prev = c;
                        counter = 1;
                    }
                    else
                    {
                        counter++;
                    }
                }
                data.push_back(v);
            }
        }

        for (int i = 0; i < data[0].size(); i++) {
            int low = 0x7FFFFFFF, hi = 0;
            for (vector<int> & v : data) {
                low = min(low, v[i]);
                hi = max(hi, v[i]);
            }
            minimum.push_back(low);
            maximum.push_back(hi);
        }
        int total = 0;
        for (int i = 0; i < minimum.size(); i++)
        {
            int low = 0x7FFFFFFF;
            for (int j = minimum[i]; j <= maximum[i]; j++)
            {
                int counter = 0;
                for (vector<int> & v : data) 
                {
                    counter += abs(v[i] - j);
                }
                low = min(low, counter);
            }
            total += low;
        }
        cout << Case(test) << total << "\n";

    endloop:;
        continue;
    }

   
    

  //  system("PAUSE");

}