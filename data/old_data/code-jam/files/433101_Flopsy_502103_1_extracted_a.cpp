#include <cstdio>
#include <cmath>

using namespace std;

int main()
{
	int t;
	int n;
	int k;

	scanf("%d", &t);

	for (int i = 0; i < t; i++)
	{
		scanf("%d %d", &n, &k);

		printf("Case #%d: ", i + 1);

		if (((k + 1) % int(pow(2, n))) == 0)
		{
			printf("ON");
		}
		else
		{
			printf("OFF");
		}

		printf("\n");
	}
}

