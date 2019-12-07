#include<bits/stdc++.h>
using namespace std;
int inverse(int key)
{
  int r1,r2,r,q,t1,t2,t;
  r1=26;r2=key;
  t1=0;t2=1;
  while(r2>0)
  {
    q=r1/r2;
    r=r1-q*r2;
    r1=r2;r2=r;
    t=t1-q*t2;
    t1=t2;t2=t;
  }
  if(r1==1)
  {
    if(t1<0)
      t1+=26;
    return t1;
  }
}
int main()
{
	string plaintext;
	int key;
	cout<<"Enter the plaintext\n";
	cin>>plaintext;

	cout<<"Enter the key\n";
	cin>>key;
	if(__gcd(26,key)!=1)
	{
		cout<<"Invalid Key!\n";
	}
	else
	{
		int l=plaintext.length();
		char ciphertext[l+1];
		for(int i=0;i<l;i++)
		{
			ciphertext[i]='A'+(char)(( ((int)plaintext[i]-'A') * key) % 26);
		}
		ciphertext[l]='\0';
		cout<<"The Encrypted Ciphertext is:\n";
		for(int i=0;i<=l;i++)
		{
			cout<<ciphertext[i];
		}
		cout<<"\n";
		/////////////////////////////////////////
		cout<<"Enter the Ciphertext\n";
		string ciphertext2;
		cin>>ciphertext2;
		//cout<<"The multiplicative inverse of " << key <<"is";
        int a_inv = 0;
        int flag = 0;
        for (int i = 0; i < 26; i++)
        {
            flag = (key * i) % 26;
            if (flag == 1)
            {
                a_inv = i;
            }
        }
        int l2=ciphertext2.length();
		char plaintext2[l2+1];
		for(int i=0;i<l2;i++)
		{
			plaintext2[i]=(char) (((a_inv * ((ciphertext2[i]+'A')) % 26)) + 'A');
		}
		plaintext2[l2]='\0';
		cout<<"\nThe Decrypted Plaintext is:\n";
		for(int i=0;i<=l2;i++)
		{
			cout<<plaintext2[i];
		}
		cout<<"\n";
	}
	return 0;
}
