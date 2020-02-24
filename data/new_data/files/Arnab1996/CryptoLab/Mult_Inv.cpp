#include<bits/stdc++.h>
using namespace std;
//Linear Method O(m)
int modInv(int a, int m)
{
	a=a%m;
	for(int i=1 ; i<m ; i++)
	{
		if( (a*i)%m==1 )
			return i;
	}
}
//Logarithmic Method O(log(m))
int e_ec(int a, int m, int *x, int *y)
{
	if(a==0)
	{
		*x==0;
		*y==1;
		return m;
	}
	int x1, y1;
	int gcd = e_ec (m%a , a , &x1 , &y1 );
	*x = y1 - (m/a)*x1;
	*y = x1;

	return gcd;
}
int main()
{
	int a,domain;
	cout<<"Enter the value of a:\n";
	cin>>a;
	cout<<"Enter the domain limit:\n";
	cin>>domain;
	cout<<"The inverse of "<<a<<" in "<<domain<<" domain is "<<modInv(a,domain)<<"\n";
	return 0;
}
