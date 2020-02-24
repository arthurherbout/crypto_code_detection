#include<bits/stdc++.h>
using namespace std;
int main()
{
	string cipher;
	cout<<"Enter the cipher-text:\n";
	cin>>cipher;
	int a[12];//Phi(26) = 1*(13-1)*(2-1) = 12
	for(int i=0,j=0 ; i<26,j<12 ; i++)
	{
		if(__gcd(26,i)==1)
		{
			a[j++]=i;
		}
	}
	for(int k2=0;k2<26;k2++)
	{
		for(int k1=0; k1<12 ; k1++)
		{

		int l=cipher.length();
		char plaintext[l+1];
		for(int i=0; i<l; i++)
		{
			plaintext[i] = (a[k1] * ( (cipher[i] - k2) %26 ) %26 + 'A');
		}
		plaintext[l] = '\0';
		for(int i=0; i<=l; i++)
		{
			cout<<plaintext[i];
		}
		cout<<" "<<k1<<" "<<k2;
		cout<<"\n";
		}
	}
}
