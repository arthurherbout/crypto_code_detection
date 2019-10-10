#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

int num[10];

string multi(string x,int y)
{
	string ret;
	for (int i = 1; i <= y; i ++)
		ret += x;
	return ret;
}

string multireplace(string ret,string x,string y,int z)
{
	for (int i = 1; i <= z; i ++)
	{
		ret = ret.replace(ret.find(x),x.length(),y);
	}
	return ret;
}

int main()
{
	int T;
	scanf("%d",&T);
	for (int t = 1; t <= T; t ++)
	{
		printf("Case #%d: ",t);
		int n;
		scanf("%d",&n);
		for (int i = 1; i <= 6; i ++)
			scanf("%d",&num[i]);
		string ans;

		if (num[1] == num[4] && num[1] > 0)
		{
			if (n - num[1] - num[4] > 0)
			{
				cout << "IMPOSSIBLE" << endl;
				continue;
			}
			else 
			{
				ans = multi("RG",num[1]);
				cout << ans << endl;
				continue;
			}
		}

		if (num[2] == num[5] && num[2] > 0)
		{
			if (n - num[2] - num[5] > 0)
			{
				cout << "IMPOSSIBLE" << endl;
				continue;
			}
			else 
			{
				ans = multi("OB",num[2]);
				cout << ans << endl;
				continue;
			}
		}

		if (num[3] == num[6] && num[3] > 0)
		{
//			printf("%d\n", n - num[3] - num[6]);
			if (n - num[3] - num[6] > 0)
			{
				cout << "IMPOSSIBLE" << endl;
				continue;
			}
			else 
			{
				ans = multi("YV",num[3]);
				cout << ans << endl;
				continue;
			}			
		}

		if (num[1] < num[4])
		{
			cout << "IMPOSSIBLE" << endl;
			continue;
		}
		else 
			num[1] -= num[4];
		if (num[5] < num[2])
		{
			cout << "IMPOSSIBLE" << endl;
			continue;
		}
		else 
			num[5] -= num[2];
		if (num[3] < num[6])
		{
			cout << "IMPOSSIBLE" << endl;
			continue;
		}
		else 
			num[3] -= num[6];

		if (num[1] >= num[3] && num[1] >= num[5])
		{
			int temp = num[3] + num[5] - num[1];
			if (temp < 0)
			{
				cout << "IMPOSSIBLE" << endl;
				continue;
			}
			ans = multi("RY",num[3] - temp) + multi("RB",num[5] - temp) + multi("RBY",temp);
		}
		else if (num[3] > num[1] && num[3] >= num[5])
		{
			int temp = num[1] + num[5] - num[3];
			if (temp < 0)
			{
				cout << "IMPOSSIBLE" << endl;
				continue;
			}
			ans = multi("YR",num[1] - temp) + multi("YB",num[5] - temp) + multi("YBR",temp);
		}
		else if (num[5] >= num[1] && num[5] >= num[3])
		{
			int temp = num[1] + num[3] - num[5];
			if (temp < 0)
			{
				cout << "IMPOSSIBLE" << endl;
				continue;
			}
			ans = multi("BR",num[1] - temp) + multi("BY",num[3] - temp) + multi("BRY",temp);
		}
		ans = multireplace(ans,"R","RGR",num[4]);
		ans = multireplace(ans,"Y","YVY",num[6]);
		ans = multireplace(ans,"B","BOB",num[2]);
		cout << ans << endl;
	}
	return 0;
}
