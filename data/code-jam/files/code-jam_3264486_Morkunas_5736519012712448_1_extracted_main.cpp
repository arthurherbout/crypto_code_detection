#include <iostream>
#include <fstream>
using namespace std;
ifstream input ("A-large.in");
ofstream out ("A-large.out");
int solve(string str, int K)
{
    int len = str.length();
    int suma = 0;
    for(int i = 0; i < len; i++)
    {
        //cout << str << endl;
        if(i+K > len)
        {
            for(int t = i; t < len; t++)
            if(str[t]=='-') return -1;
                return suma;
        }
        if(str[i]=='-')
        {
            for(int j = i; j < i+K; j++)
            {
                if(str[j]=='-')str[j]='+';
                else str[j]='-';
            }
            suma++;
        }

    }

}

int main()
{
    int N,K,ans;
    string str;
    input >> N;
    for(int i = 0; i < N; i++)
    {
        input >> str >> K;
        ans = solve(str,K);
        out << "Case #" << i+1 <<": ";
        if(ans == -1){
            out << "IMPOSSIBLE"<< endl;
        }
        else
        {
            out << ans << endl;
        }
    }
    //Case #x: y

    return 0;
}
