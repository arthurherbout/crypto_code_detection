#include<bits/stdc++.h>
using namespace std;
int p = 23;
int d = 10;
int e1 = 5;
int r = 4;
int main()
{
	int e2 = fmod(pow(e1, d), p);
	long c1 = fmod(pow(e1, r), p);
	long pt;
	cout<<"Enter the plain text\n";
	cin>>pt;
	cout<<"The cipher text is\n";
	int c2  = fmod(pt*pow(e2, r), p);
	cout<<fmod(pt*pow(e2, r), p)<<"\n";
	/////////////////////////////////
	cout<<"The original text was: \n";
	cout<<"(c1^d) mod p = 4 "<<endl;
	cout<<fmod((c2*4), p);
	return 0;
}
