#include<bits/stdc++.h>
using namespace std;
int mul_inv(int a, int b)
{
	int b0 = b, t, q;
	int x0 = 0, x1 = 1;
	if (b == 1)
        return 1;
	while (a > 1)
    {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
        return x1;
}
int chinese_remainder(int *n, int *a, int len)
{
	int p, i, prod = 1, sum = 0;
	for (i = 0; i < len; i++) prod *= n[i];
	for (i = 0; i < len; i++)
    {
		p = prod / n[i];
		sum += a[i] * mul_inv(p, n[i]) * p;
	}
	return sum % prod;
}
int main()
{
    cout<< "Enter the number of relations:\n";
    int n;
    cin>> n;
    int num[n], rem[n];
    cout<< "Enter the " <<n<< " values of quotients and remainders:\n";
    for(int i=0; i<n; i++)
    {
        cin>>num[i]>>rem[i];
    }
    cout<< "Hence, the congruence relations are as follows:\n";
    for(int i=0; i<n; i++)
    {
        cout<< " x = " << num[i] << " mod " << rem[i] << "\n";
    }
    cout<<"\nSolution:..\n";
    int M = 1;
    for(int i=0;i<n;i++)
        M *=rem[i];
    cout<<"The value of M = "<<M<<"\n";
    for(int i=0;i<n;i++)
    {
        cout<<"The value of m"<<i+1<<" is "<<M/rem[i]<<"\n";
    }
    cout << "\nThe value of x is " << chinese_remainder(rem, num, n) << "\n";
    return 0;
}
