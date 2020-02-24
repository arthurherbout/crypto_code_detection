#include<stdio.h>

int graph[2001][2001];
int wall[2001];
int N, M;

int min_cycle(int r)
{
  int i, min;

  min = N;

  for(i = 0; i < N; i++)
    {
      if(graph[r][i])
	{
	  int curr, l;

	  curr = i;
	  l = 1;

	  while(curr != r)
	    {
	      int j;

	      for(j = r; 1; j = (j - 1 + N) % N)
		if(graph[curr][j])
		  if(j == r && l == 1)
		    continue;
		  else
		  {
		    curr = j;
		    l++;
		    break;
		  }
	    }

	  if (l < min)
	    min = l;
	}
    }

  return min;
}

int compute()
{
  int min, i;

  min = N;

  for(i = 0; i < N; i++)
    {
      int l;

      l = min_cycle(i);

      if (l < min)
	min = l;
    }

  return min;
}

int main()
{
  int T, t;

  scanf("%d\n", &T);

  for(t = 1; t <= T; t++)
    {
      int i, j;

      scanf("%d %d\n", &N, &M);

      for(i = 0; i < N; i++)
	for(j = 0; j < N; j++)
	  graph[i][j] = 0;

      for(i = 0; i < N - 1; i++)
	{
	  graph[i][i + 1] = 1;
	  graph[i + 1][i] = 1;
	}

      graph[0][N - 1] = 1;
      graph[N - 1][0] = 1;

      for(i = 0; i < M; i++)
	{
	  scanf("%d", wall + i);
	  wall[i]--;
	}

      for(i = 0; i < M; i++)
	{
	  int v;

	  scanf("%d", &v);
	  v--;

	  graph[wall[i]][v] = 1;
	  graph[v][wall[i]] = 1;
	}

      printf("Case #%d: %d\n", t, compute());
    }
}
