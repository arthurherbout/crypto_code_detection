// Harmony.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <algorithm>
using namespace std;
void swap(__int64 &a, __int64 &b) {
	__int64 tmp;
	tmp =a ;
	a= b;
	b = tmp;
}
int gcd(__int64 a, __int64 b)
{
	if (a < b)
		swap(a, b);

	if (b == 0)
		return a;
	else
		return gcd(b, a%b);
}
__int64 data[10001];
int _tmain(int argc, _TCHAR* argv[])
{
	freopen("C-small-attempt2.in","r",stdin);
	freopen("result","w",stdout);
	int cas, tcas = 0;
	bool yes;
	cin>>cas;
	while(tcas <cas) {
		yes = true;
		tcas++;
		__int64 n, l, h;
		cin>>n>>l>>h;
		int i;
		for(i = 0 ; i <n ; i++) {
			cin>>data[i];
		}
		if(l == 1){
			cout<<"Case #"<<tcas<<": 1"<<endl;
			continue;
		}

		sort(data, data+n);
		__int64 tmp, ret = 1;
		tmp = data[0];


		i=1;
		
		for(i = 1 ; i<n&&data[i] < l ; i++) {
			tmp = gcd(tmp, data[i]);
		}
		
		ret =tmp;
		int j; 
 		for(j = 0 ;j  < i ; j++) {
			ret*=data[j];
			ret/=tmp;
			if(ret>h)
			{
				yes = false;
				break;
			}
		}			

		if(yes) 
		{
			if(ret<l&&i<n) {
				int tmp1 = gcd(ret, data[i]);
				ret = ret * data[i]/tmp1/tmp1;
			}

			for(j = i ; j<n ; j++) {
				
				if(data[j]%ret!=0)
				{
					int tmp = gcd(ret, data[j]);
					if(ret*data[j]/tmp<h)
						ret = ret*data[j]/tmp;
					else{
						yes= false;
						break;
					}
				}
			}
		}
		
		if(ret>h){
			yes = false;
		}
		else{
			if(l>ret&&l%ret==0)
				ret = l;
			else if(h/ret-l/ret>0)
			{
				ret = ret*(l/ret+1);
			}
			else if(h%ret==0&&h>ret){
				ret = h;
			}
			else yes = false;	
				
		}
		cout<<"Case #"<<tcas<<": ";
		if(yes)
			cout<<ret<<endl;
		else cout<<"NO"<<endl;
	}
	return 0;
}

