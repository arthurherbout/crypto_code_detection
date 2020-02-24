#include<bits/stdc++.h>
using namespace std;

int main()
{
    string plaintext, ciphertext, key, lpt, rpt, rpt_mod, rpt_exp, rpt_left, rpt_right;
    int x_left, y_left, x_right, y_right;
    cout<<"Enter the 16 bit plain text:\n";
    cin>>plaintext;
    cout<<"Enter the 10 bit Key:\n";
    cin>>key;

    lpt = plaintext.substr(0, 8);
    rpt = plaintext.substr(8);

    rpt_mod = rpt[7] + rpt + rpt[0];

    //Exor
    for(int i = 0 ; i < 10 ; i++ )
    {
        if ( rpt_mod[i] == key[i] )
            rpt_exp = rpt_exp + '0';
        else
            rpt_exp = rpt_exp + '1';
    }

    rpt_left = rpt_exp.substr(0, 5);
    rpt_right = rpt_exp.substr(5);

    x_left = ('0' - rpt_left[0])*2 + ('0' - rpt_left[4]);
    y_left = ('0' - rpt_left[1])*4 + ('0' - rpt_left[2])*2 + ('0' - rpt_left[3]);

    x_right = ('0' - rpt_right[0])*2 + ('0' - rpt_right[4]);
    y_right = ('0' - rpt_right[1])*4 + ('0' - rpt_right[2])*2 + ('0' - rpt_right[3]);

    string SBox[4][8] =
    {   {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111"},
        {"1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"},
        {"1111", "1110", "1101", "1100", "1011", "1010", "1001", "1000"},
        {"0111", "0110", "0101", "0100", "0011", "0010", "0001", "0000"}    };

    ciphertext = lpt + SBox[-x_left][-y_left] + SBox[-x_right][-y_right];
    cout<<"The Encrypted cipher text is: \n"<<ciphertext;

    return 0;
}

/*
Plain text: 0000000000100000
Key: 1111111111
*/
