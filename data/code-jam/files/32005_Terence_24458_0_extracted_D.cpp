#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#define N 110

struct node 
{
  int s,t;
}p[N], q[N];
  
int n, m, seq[N];
bool map[N][N];

int comp(const void *a, const void *b)
{
  return *(int *)b - *(int *)a;
}

bool subtree(int seq[])
{
  for(int i = 0; i < m-1; i++)
    if(!map[seq[q[i].s]][seq[q[i].t]])return false;
  return true;
}

int main()
{
  int t, index, i;
  bool r;
  
  scanf("%d", &t);
  for(index = 1; index <= t; index++)
  {
    scanf("%d", &n);
    memset(map, false, sizeof(map));
    r = false;
    for(i = 0; i < n-1; i++) 
    {
      scanf("%d%d", &p[i].s, &p[i].t);
      map[p[i].s-1][p[i].t-1] = true;
      map[p[i].t-1][p[i].s-1] = true;
    }
    scanf("%d", &m);
    for(i = 0; i < m-1; i++)
    {
      scanf("%d%d", &q[i].s,&q[i].t);
      q[i].s--;
      q[i].t--;
    }
    for(i = 0; i < n; i++)
      seq[i] = i;
    do 
    {
      if(m < n)
        qsort(seq+m,n-m,sizeof(int), comp);
      //      for(i = 0; i < m; i++)
      //        printf("%d ", seq[i]);
      //      printf("\n");
      if(subtree(seq)) 
      {
        r = true;
        break;
      }
    }while(std::next_permutation(seq, seq+n));
    printf("Case #%d: %s\n", index, r?"YES":"NO");
  }
  
  return 0;
}
