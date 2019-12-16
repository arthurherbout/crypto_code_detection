#include <iostream>
using namespace std;
typedef unsigned long long ll;
ll p = 397, q = 401;
ll n = p * q;
ll eulerN = (p - 1) * (q - 1);
ll e = 343;
ll d;
ll cffs, cffb;
void calcInverse(ll a, ll b)
{
        ll newa = b;
        ll newb = a % b;
        if(newa % newb == 0)
        {
                cffs = (a / b) * (-1);
                cffb = 1;
                return;
        }
        calcInverse(b, a % b);
        ll store = cffs;
        cffs = cffb - (a / b) * (cffs);
        cffb = store;
}
ll encrypt(ll plaintext, ll e)
{
        long long int ans = 1;
        while(e)
        {
                if(e & 1)
                        ans = (ans * plaintext) % n;
                plaintext = (plaintext * plaintext) % n;
                e = e >> 1;
        }
        return ans;
}
int main()
{
        int i;
        string str;
        cout<<"Enter the message:\n";
        cin >> str;
        string output = "";
        string output2 = "";
        long long int plaintext = 0;
        for(i = 0; i < str.size(); i++)
                plaintext = (plaintext * 26 + str[i] - 65) % n;
        unsigned long long int ciphertext = encrypt(plaintext, e);
        cout <<"Encrypted data :\n" <<ciphertext << endl;
        long long int temp = ciphertext;
        while(temp)
        {
                int rem = temp % 26;
                string s = "";
                s.push_back(rem + 65);
                output = s + output;
                temp = temp / 26;
        }
        calcInverse(eulerN, e);
        d = cffs;
        if(d < 0)
                d += eulerN;
        ll decrypted = encrypt(ciphertext, d);
        temp = decrypted;
        while(temp)
        {
                int rem = temp % 26;
                string s = "";
                s.push_back(rem + 65);
                output2 = s + output2;
                temp = temp / 26;
        }
        cout << "Original Message Sent :\n"<<output2 << endl;
        return 0;
}
