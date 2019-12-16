#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 10000
#define INF 9999999
int n, m;

int k, x[N], y[N];
int wmax, wmin, hmax, hmin;

bool outside(int w, int h, int pw, int ph)
{
  if(pw > wmax) 
  {
    if(ph > hmax)
    {
      return w >= pw && h >= ph;
    }
    else if(ph < hmin)
    {
      return w >= pw && h <= ph;
    }
    else
    {
      return w >= pw;
    }
  }
  else if(pw < wmin)
  {
    if(ph >= hmax)
    {
      return w <= pw && h >= ph;
    }
    else if(ph < hmin)
    {
      return w <= pw && h <= ph;
    }
    else
    {
      return w <= pw;
    }
  }
  else
  {
    if(ph > hmax) 
    {
      return h >= ph;
    }
    else
    {
      return h <= ph;
    }
  }
}

int main()
{
  int t, index;
  int w, h;
  char s[30];
  
  scanf("%d", &t);
  for(index = 1; index <= t; index++)
  {
    scanf("%d", &n);
    fgets(s, 30, stdin);
    k = 0;
    hmax = wmax = -INF;
    hmin = wmin = INF;
    for(int i = 0; i < n; i++) 
    {
      scanf("%d%d%s", &h, &w, &s);
      if(s[0] == 'B') 
      {
        if(h < hmin) hmin = h;
        if (h > hmax) hmax = h;
        if(w < wmin) wmin = w;
        if (w > wmax) wmax = w;
      }
      else 
      {
        x[k] = w;
        y[k++] = h;
      }
      
      fgets(s, 30, stdin);
    }
    //printf("k=%d w:(%d,%d), h:(%d,%d)\n", k, wmin,wmax,hmin,hmax);
    
    
    printf("Case #%d:\n", index);
    scanf("%d", &m);
    for(int i = 0; i < m; i++)
    {
      scanf("%d%d", &h, &w);
      if(wmin > wmax || hmin > hmax) 
      {
        int j;
        
        for(j = 0; j < k; j++)
          if(w == x[j] && h == y[j])
            break;
        if(j == k) 
        {
          printf("UNKNOWN\n");
        }
        else
        {
          printf("NOT BIRD\n");
        }
      }
      else if(w >= wmin && w <= wmax && h >= hmin && h <= hmax) 
      {
        printf("BIRD\n");
      }
      else 
      {
        int j;
        for(j = 0; j < k; j++)
          if(outside(w, h, x[j], y[j]))
            break;
        if(j == k) 
        {
          printf("UNKNOWN\n");
        }
        else
        {
          printf("NOT BIRD\n");
        }
      }
      
    }
  }
  
  return 0;
}
