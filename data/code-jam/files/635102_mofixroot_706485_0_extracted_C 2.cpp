#include<iostream>

using namespace std;

int cnt ;
char mat[2][111][111];
int row, col;

bool Check()
{
     int i, j;
     for(i = 1; i <= row; i++)
     {
           for(j = 1; j <= col; j++)
           {
                 if(mat[cnt&1][i][j]==1)
                    return true;
           }
     }
     return false;
}
int main()
{
    freopen("C-small-attempt0.in", "r", stdin);
    freopen("C-small-attempt0.out", "w", stdout);
    
    int cas, icas;
    cin >> cas;
    int i, j;
    for(icas = 1; icas <= cas; icas++)
    {
             cout << "Case #" << icas << ": ";
             memset(mat, 0, sizeof(mat));
             int r;
             cin >> r;
             int r1, c1, r2, c2;
             row = 0, col = 0;
             while(r--)
             {
                       cin >> c1 >> r1 >> c2 >> r2;
                       if(c1 > col) col = c1;
                       if(c2 > col) col = c2;
                       if(r1 > row) row = r1;
                       if(r2 > row) row = r2;
                       for(i = r1; i <= r2; i++)
                       {
                             for(j = c1; j <= c2; j++)
                             {
                                   mat[0][i][j] = 1;
                             }
                       }
             }
             cnt = 0;
             while(Check())
             {
                           for(i = 1; i <= row; i++)
                           {
                                 for(j = 1; j <= col; j++)
                                 {
                                       if(mat[cnt&1][i][j]==0)
                                       {
                                                              if(mat[cnt&1][i-1][j]==1 && mat[cnt&1][i][j-1]==1)
                                                                  mat[(cnt+1)&1][i][j] = 1;
                                                              else
                                                                  mat[(cnt+1)&1][i][j] = 0;
                                       }
                                       else
                                       {
                                               if(mat[cnt&1][i-1][j]==0 && mat[cnt&1][i][j-1]==0)
                                                                  mat[(cnt+1)&1][i][j] = 0;
                                               else
                                                                  mat[(cnt+1)&1][i][j] = 1;
                                       }
                                 }
                           }
                           cnt++;
             }
             printf("%d\n", cnt);
             
    }
    return 0;
}
