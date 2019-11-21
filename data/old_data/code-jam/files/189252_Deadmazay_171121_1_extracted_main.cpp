#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
using namespace std;

char mp[1000];

set <char> st;

vector <int> v;

int f[1000];

int main ()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	
	for (int i=0; i<1000; i++)
	{
		f[i] = i;
	}
	
	f[2] = 1;
	f[1] = 2;
	
	int n;
	cin >> n;
	for (int j=0; j<n; j++)
	{
		
		for (int i=0; i<300; i++)
		{
			mp[i] = 0;
		}
		
		v.clear();
		
	
		string s;
		cin >> s;
	
		int k = 0;
	
		for (int i=0; i<s.length(); i++)
		{
			if (!mp[s[i]])
			{
				k++;
				mp[s[i]] = f[k]; 
			}
			
			v.push_back(mp[s[i]]);
		}
	
		k = max(k, 2);
		
		unsigned long long ans = 0;
	
		for (int i=0; i<v.size(); i++)
		{
			ans = ans * k + v[i] - 1;
		}
		
		cout << "Case #" << j + 1 << ": " << ans << endl;
	}
	
    return 0;
}
