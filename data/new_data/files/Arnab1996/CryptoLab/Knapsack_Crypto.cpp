#include<bits/stdc++.h>
using namespace std;
int r = 37;
int n = 900;
int b[] = {7, 11, 19, 39, 7, 157, 153};
int k = 7;
int perm[] = {4, 6, 3, 7, 2, 5, 1};
int main()
{
    cout<<"The message is: ";
    int m[] = {0, 1, 0, 0, 1, 1, 1};
    for(int i : m)
        cout<<i<<" ";
    cout<<"\n";
    long long cipher = 0;
    long t[7];
    for(int i = 0 ; i < 7 ; i++)
        t[i] = fmod(r*b[i], n);
    for(int i = 0 ; i < 7 ; i++)
        cipher += (t[i] * m[i]);
    cout<<"The cipher text is:  "<<cipher<<"\n";
    /////////////////////////////////////////////
    cout<<"The inverse of r = 37 is 73 in modulo 900\n";
    int s_dash = fmod(cipher*73, n);
    cout<<"The value of S-dashed is in  modulo 900 is "<<s_dash<<"\n";
    int x[7];
    for(int i = 6 ; i >= 0 ; i--)
    {
        if(s_dash >= b[i])
        {
            x[i] = 1;
            s_dash = s_dash - b[i];
        }
        else
            x[i] = 0;
    }

    cout<<"Original message was ";
    for(int i : x)
        cout<<i<<" ";
    cout<<"\n";
    return 0;
}
