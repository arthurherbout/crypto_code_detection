#include<iostream>
#include<algorithm>

using namespace std;

int n;
char mat[222][222];

bool check1(int r, int c1, int c2)
{
     int mc = (c1+c2)/2;
     int k = c2-mc+1;
     int i, j;
     for(i = 1; i <= r; i++)
     {
           for(j = c1; j <= c2; j++)
           {
                 if(abs(i-r)+abs(j-mc)+1 <= k)
                 {
                       if(mat[i][j] != mat[i][2*mc-j] && j <= mc)
                       {
                                    return false;
                       }
                       if(mat[i][j] != mat[2*r-i][j])
                          return false;
                       if(mat[i][j] != mat[2*r-i][2*mc-j])
                          return false;
                 }
           }
     }
     return true;
}
bool check2(int r1, int r2, int c)
{
     int mr = (r1+r2)/2;
     int k = r2-mr+1;
     int i, j;
     for(i = r1; i <= mr; i++)
     {
           for(j = 1; j <= n; j++)
           {
                 if(abs(i-mr)+abs(j-c)+1 <= k)
                 {
                       if(mat[i][j] != mat[i][2*c-j] && j <= c)
                       {
                                    return false;
                       }
                       if(mat[i][j] != mat[2*mr-i][j])
                          return false;
                       if(mat[i][j] != mat[2*mr-i][2*c-j])
                          return false;
                 }
           }
     }
     return true;
}


int main()
{
    freopen("A-small-attempt0.in", "r", stdin);
    freopen("A-small-attempt0.out", "w", stdout);
    
    int cas, icas;
    int i, j, k;
//    cin >> cas;
    scanf("%d", &cas);
    getchar();
    for(icas = 1; icas <= cas; icas++)
    {
             cout << "Case #" << icas << ": ";
             scanf("%d", &n);
             getchar();
             memset(mat, 0, sizeof(mat));
             for(i = 1; i <= 2*n-1; i++)
             {
                   gets(mat[i]+1);
             }
             //◊Û 
             int maxk = 0;
             for(k = 1; k <= n; k++)
             {
                   if(check1(n, 1, 2*k-1))
                   {
                               if(k > maxk)
                                 maxk = k;
                   }
             }
             //”“ 
             for(k = 1; k <= n; k++)
             {
                   if(check1(n, 2*n-1 - (2*k-1) + 1, 2*n-1))
                   {
                               if(k > maxk)
                                 maxk = k;
                   }
             }
             for(k = 1; k <= n; k++)
             {
                   if(check2(1, k, n))
                   {
                               if(k > maxk)
                                 maxk = k;
                   }
             }
             for(k = 1; k <= n; k++)
             {
                   if(check2(n-k+1, n, n))
                   {
                               if(k > maxk)
                                 maxk = k;
                   }
             }
          //   cout << maxk << endl;
             int ans = (2*n-maxk)*(2*n-maxk)-n*n;
             cout << ans << endl;
    }
    return 0;
}
