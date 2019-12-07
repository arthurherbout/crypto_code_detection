#include<bits/stdc++.h>
using namespace std;
int main()
{
	string plaintext;
	int key;
	cout<<"Enter the plaintext\n";
	cin>>plaintext;
	cout<<"Enter the key\n";
	cin>>key;
	int l=plaintext.length();
	char ciphertext[l+1];
	for(int i=0;i<l;i++)
	{
		ciphertext[i]='A'+(char)(( (int)plaintext[i]-'A' + key) % 26);
	}
	ciphertext[l]='\0';
	cout<<"The Encrypted Ciphertext is:\n";
	for(int i=0;i<=l;i++)
	{
		cout<<ciphertext[i];
	}
	cout<<"\n";
	/////////////////////////////////////
	cout<<"Enter the Ciphertext:\n";
	string ciphertext2;
	cin>>ciphertext2;
	int l2=plaintext.length();
	char plaintext2[l2+1];
	for(int i=0;i<l2;i++)
	{
		plaintext2[i]='A'+(char)(( (int)ciphertext2[i]-'A' - key) % 26);
	}
	plaintext2[l2]='\0';
	cout<<"The Decrypted Plaintext is:\n";
	for(int i=0;i<=l2;i++)
	{
		cout<<plaintext2[i];
	}
	cout<<"\n";
	return 0;
}
