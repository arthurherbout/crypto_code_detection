#include<bits/stdc++.h>
using namespace std;

long long btoi(string s)
{
    long long ans=0;
    for(long long i = s.length() - 1 ; i >= 0 ; i--)
        ans+=(s[i]-'0') * pow(2, s.length()-i-1);
    return ans;
}

string itob(long long number)
{
    if ( number == 0 )
        return "0";
    if ( number == 1 )
        return "1";
    if ( number % 2 == 0 )
        return itob(number / 2) + "0";
    else
        return itob(number / 2) + "1";
}


string mul(string a, string b)
{
    string s = itob( btoi(a) * btoi(b));
    return s.substr(s.length()-17, 16);
}

string add(string a,string b)
{
    string s=itob(btoi(a) * btoi(b));
    return s.substr(s.length()-17, 16);
}

string xo(string a,string b)
{
    string s=itob(btoi(a) * btoi(b));
    return s.substr(s.length()-17, 6);
}

int main()
{
    string ip = "1010010100000101000101010110000000100001111111100010000001101111";
    string key = "00000001001001010000010010111100010101000111000110001001010001100101110000100101100011111000101100110110011111101011000001000110 ";
    string new_key = key.substr(25,103) + key.substr(0,25) ;

    string x1 = ip.substr(0,16);
    string x2 = ip.substr(16,16);
    string x3 = ip.substr(32,16);
    string x4 = ip.substr(48,16);

    string k1 = new_key.substr(0,16);
    string k2 = new_key.substr(16,16);
    string k3 = new_key.substr(32,16);
    string k4 = new_key.substr(48,16);
    string k5 = new_key.substr(64,16);
    string k6 = new_key.substr(80,16);

    string st1 = mul(x1,k1);
    string st2 = add(x2,k2);
    string st3 = add(x3,k3);
    string st4 = mul(x4,k4);
    string st5 = xo(st1,st3);
    string st6 = xo(st2,st4);
    string st7 = mul(st5,k5);
    string st8 = add(st6,st7);
    string st9 = mul(st8,k6);
    string st10 = add(st7,st9);
    string st11 = xo(st5,st9);
    string st12 = xo(st3,st9);
    string st13 = xo(st7,st9);
    string st14 = xo(st1,st10);

    string r1 = mul(st11, k1);
    string r2 = mul(st12, k2);
    string r3 = mul(st13, k3);
    string r4 = mul(st14, k4);

    string op = r1+r2+r3+r4;
    cout<<"The Input is: "<<btoi(ip)<<"\n";
    cout<<"The Output is (in binary): "<<op<<"\n";
    cout<<"The Output is (in decimal): "<<btoi(op)<<"\n";

    return 0;
}
