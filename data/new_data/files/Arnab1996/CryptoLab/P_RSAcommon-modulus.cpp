#include <bits/stdc++.h>
using namespace std;

void getString(long long int val)
{
        if(val==0)
                return;
        getString(val/26);
        char ch = 'A'+val%26;
        cout<<ch;
}


void decrypt(long long int cp,long long int p,long long int q,long int e)
{
        long long int phi = (p-1)*(q-1);
        long long int i = 1;
        long long int n = p*q;
        while(1)
        {
                if((i*e)%phi==1)
                        break;
                i++;
        }
        long long int val = 1;
        long long int d = i;
        for(i=0; i<d; i++)
        {
                val = (val*cp)%n;
        }
        getString(val);
}

int main()
{
        long long int p = 397;
        long long int q = 401;
        long long int e = 343;
        long long int cp;
        cin>>cp;
        decrypt(cp, p, q, e);
        return 0;
}