#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int t;
	
	cin >> t;
	
	for (int i = 0; i < t; i++)
	{
		int n;
		
		cin >> n;
		
		vector<int> candies(n);
		
		for (int j = 0; j < n; j++)
		{
			cin >> candies[j];
		}
		
		sort(candies.begin(), candies.end());
		
		int sean_actual = -1;
		
		for (int j = 1; j < n; j++)
		{
			int patrick = 0;
			
			for (int k = 0; k < j; k++)
			{
				patrick ^= candies[k];
			}
			
			int sean = 0;
			
			for (int k = j; k < n; k++)
			{
				sean ^= candies[k];
			}
			
			if (patrick == sean)
			{				
				sean_actual = 0;
				
				for (int k = j; k < n; k++)
				{
					sean_actual += candies[k];
				}
				
				break;
			}
		}
		
		cout << "Case #" << (i + 1) << ": ";
		
		if (sean_actual == -1)
		{
			cout << "NO\n";
		}
		else
		{
			cout << sean_actual << '\n';
		}
	}
}
