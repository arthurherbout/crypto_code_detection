#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

char c[500];
char str[] = "welcome to code jam";
int n;
int nums[501][21];
char buf[1000];

int main()
{
	int cases = 0;
	scanf("%d", &n);
	gets(buf);
	while(n--)
	{
		gets(buf);
		int len = strlen(buf);
		fill(nums[0], nums[501], 0);
		for (int i = 0; i <= len; ++i) nums[i][0] = 1;
		for (int i = 0; i < len; ++i)
		{
			for (int j = 0; j < 19; ++j)
			{
				nums[i + 1][j + 1] = nums[i][j + 1];
				if (buf[i] == str[j]) nums[i + 1][j + 1] += nums[i][j];
				nums[i + 1][j + 1] %= 10000;
			}
		}
		printf("Case #%d: %04d\n", ++cases, nums[len][19]);
	}
}