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
	int l = a.size(), i;
    string s;
    bool carry = false;
    for (i = 0; i < l; ++i)
    {
		if (a[i] == '1' && b[i] == '1')
		{
			s += (carry?'1':'0');
			carry = true;
		}
		else
		if ((a[i] == '0' && b[i] == '1')||(a[i] == '1' && b[i] == '0'))
		{
			if (carry)
				s += '0';	// carry is still true
			else
				s += '1';	// carry is still false
		}
		else
		{
			s += (carry?'1':'0');
			carry = false;
		}
	}
	return s;
}

string xo(string a,string b)
{
    int l = a.size(), i;
    string s;
    for (i = 0; i < l; ++i)
    {
		s += ( a[i] == b[i] ? '0' : '1' );
	}
	return s;
}

void round(string &x1, string &x2, string &x3, string &x4, string k1, string k2, string k3, string k4, string k5, string k6, int r)
{
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
    string st11 = xo(st1,st9);
    string st12 = xo(st3,st9);
    string st13 = xo(st2,st10);
    string st14 = xo(st4,st10);

    string y1 = st11, y2 = st12, y3 = st13, y4 = st14;
    if (r < 8)
    {
		y2.swap(y3);
	}
	else
	{
		y1 = mul(y1, k1);
		y2 = add(y2, k2);
		y3 = add(y3, k3);
		y4 = mul(y4, k4);
	}
	x1 = y1; x2 = y2; x3 = y3; x4 = y4;
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

    for (int i = 1; i <= 8; ++i)
    {
		round(x1, x2, x3, x4, k1, k2, k3, k4, k5, k6, i);
	}

    string op = x1+x2+x3+x4;
    cout<<"The Input is (in binary): "<<ip<<"\n";
    cout<<"The Key is (in binary): "<<key<<"\n";
    cout<<"The Output is (in binary): "<<op<<"\n";

    return 0;
}
