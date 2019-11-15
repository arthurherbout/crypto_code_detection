#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

bool ugly(long long n)
{
//    if(n<=0)
//        return true;

    long long primes[4] = {2,3,5,7};
    for(int i=0; i<4; i++)
    {
        if(n%primes[i] == 0)
            return true;
    }
    return false;
}

long long cut(string str, long long current, bool plus)
{
    int ret=0;
    for(int i=1; i<=str.size(); i++)
    {
        string a=str.substr(0, i),
            b=str.substr(i);
//        cout << "cut: " << str << "->" << a << ", " << b<<endl;

        long long n;
        sscanf(a.c_str(), "%lld", &n);

        long long tmp=current+(plus?n:-n);

        if(b.empty())
        {
            if(ugly(tmp))
            {
//                cerr << tmp << endl;
                ret++;
            }
        }
        else
        {
            ret+=cut(b, tmp, true);
            ret+=cut(b, tmp, false);
        }
    }
    return ret;
}

int main()
{
    int n;
    cin >> n;

    for(int i=0; i<n; i++)
    {
        string str;
        cin >> str;
        
        cout << "Case #" << i+1 << ": " << cut(str, 0, true) << endl;
    }
    return 0;
}
