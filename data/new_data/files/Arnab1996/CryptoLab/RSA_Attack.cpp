#include<bits/stdc++.h>
using namespace std;
typedef long double ll;
int main()
{
    ll p = 397, q = 401, e = 343, c = 1; //GCD(e, Phi(n)) = 1
    ll  n = p*q ; //Phi(n) = 12
    for(ll i = 2 ; i < n ; i++)
        if( fmod(pow(i, e), n) == c)
            cout<<"The original message is : "<< i <<endl;
    return 0;
}
