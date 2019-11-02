#include<stdio.h>

#define LL long long
#define MAX 100

char a[210][210];
int X,Y;

int getmin(int a,int b,int c,int d)
{
    int min = a;
    if(b < min)
         min = b;
    if(c < min)
         min = c;
    if(d < min)
         min = d;
    
    return min;
}

int mark(int e,int w,int n,int s,int min,int x,int y)
{
    x += 100;
    y += 100;
    
    if(e == min)
              a[x][y] = 'e';
    else
    if(w == min)
              a[x][y] = 'w';
    else
    if(n == min)
              a[x][y] = 'n';
    else
    if(s == min)
              a[x][y] = 's';
}


int findRout(int x,int y,int step)
{
     int e,w,n,s, min;
     e = w = n = s = MAX;
     if(step > 10)
      return 0;
     if(X == x  && Y == y)
             return step;
     
     if(x < X && abs(x+step) < MAX)
                    e = findRout(x+step,y,step+1);
     if(x > X && abs(x-step) < MAX)
                    w = findRout(x-step,y,step+1);
     if(y < Y && abs(y+step) < MAX)
                    n = findRout(x,y+step,step+1);
     if(y > Y && abs(y-step) < MAX)
                    s = findRout(x,y-step,step+1);
     
     min = getmin(e,w,n,s);
     
     mark(e,w,n,s,min,x,y); 
     
     return min; 
}


int main()
{
    int t,kas,i,j;
    
    int x,y,step;
    
    scanf("%d",&t);
    
    for(i=0;i<202;i++)
                      for(j=0;j<202;j++)
                                        a[i][j] = ' ';
    for(kas = 1; kas <= t; kas++)
    {
        
            scanf("%d %d",&X,&Y);    
            findRout(0,0,1);
            x = y = 0;
            printf("Case #%d: ",kas);  
            for(step = 1;!(x==X && y == Y);step++)
            {
                     if(a[x+100][y+100] == 'e')
                     {
                                printf("E");
                                x += step;
                     }
                     else
                     if(a[x+100][y+100] == 'w')
                     {
                                printf("W");
                                x -= step;
                     }
                     else
                     if(a[x+100][y+100] == 'n')
                     {
                                printf("N");
                                y += step;
                     }
                     else
                     if(a[x+100][y+100] == 's')
                     {
                                printf("S");
                                y -= step;
                     }
                     else
                         break;
            }
               
            printf("\r\n");        
    }
    
    return 0;
}


              
              
              
              

