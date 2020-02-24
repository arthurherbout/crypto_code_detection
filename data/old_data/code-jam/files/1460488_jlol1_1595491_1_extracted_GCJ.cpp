#include <cstdio>
#include <list>
using namespace std;

int cases = 0;
int i = 0;
int j = 0;
int* googlers = 0;
int* surprising = 0;
int* p = 0;
int scores[100][100];
int individualScore;
int current;


int solution;
int* solutions;

int main()
{
	scanf("%d",&cases);
	solutions = new int[cases];
	googlers = new int[cases];
	surprising = new int[cases];
	p = new int[cases];

	for (i = 0; i < cases; ++i)
	{
		solution = 0;

		scanf("%d", &googlers[i]);
		scanf("%d", &surprising[i]);
		scanf("%d", &p[i]);

		for (j = 0; j < googlers[i]; ++j)
		{
			scanf("%d", &scores[i][j]);
		}

		solutions[i] = solution;
	}

	for (i = 0; i < cases; ++i)
	{
		for (j = 0; j < googlers[i]; ++j)
		{
			current = scores[i][j];
			int third = current / 3;
			int modulo = current % 3;

			if (p[i] == 0)
			{
				++solutions[i];
				continue;
			}
			else if ((current == 0) || (third + 2) < p[i])
			{
				continue;
			}
			else if (third >= p[i] || ((third + 1) >= p[i] && (modulo > 0)))
			{
				++solutions[i];
				continue;
			}
			else if (surprising[i] > 0 && (modulo == 2 || (p[i] - third <= 1)))
			{
				--surprising[i];
				++solutions[i];
				continue;
			}
		}
	}


	for (i = 0; i < cases; ++i)
	{
		printf("Case #%d: %d\n", i+1, solutions[i]);
	}

	return 0;
}
