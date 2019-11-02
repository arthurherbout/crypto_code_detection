#include <iostream>

using namespace std;

const int NN = 550;
int A[NN][NN];
int aa[NN],cc[NN];
char s[NN];
int m,n;
int C[NN][NN];
int num(char x)
{
    if(x<='9'&&x>='0')return x-'0';
    return x-'A'+10;
}
bool ok(int x,int y,int z)
{
    if(x+z-1>=m||y+z-1>=n)return false;
    for(int i=x;i<x+z;++i)
    {
        for(int j=y;j<y+z;++j)
        {
            if(A[i][j]==-1)return false;
            if(i<x+z-1)
            {
                if(A[i][j]==A[i+1][j])return false;
            }
            if(j<y+z-1)
            {
                if(A[i][j]==A[i][j+1])return false;
            }
        }
    }
    for(int i=x;i<x+z;++i)
    {
        for(int j=y;j<y+z;++j)
        {
            A[i][j]=-1;
        }
    }
    return true;
}
int main()
{
    freopen("c.in","r",stdin);
    freopen("c.out","w",stdout);
    int TTT;
    int Test = 1;
    scanf("%d",&TTT);
    while(TTT--)
    {
        scanf("%d%d",&m,&n);
        for(int i=0;i<m;++i)
        {
            scanf("%s",s);
            for(int j=0;j<n/4;++j)
            {
                int x = num(s[j]);
                A[i][j*4]=(x>>3)&1;
                A[i][j*4+1]=(x>>2)&1;
                A[i][j*4+2]=(x>>1)&1;
                A[i][j*4+3]=(x)&1;
               // printf("%d%d%d%d",A[i][j*4],A[i][j*4+1],A[i][j*4+2],A[i][j*4+3]);
            }
           // puts("");
        }

        int ans = 0;
       // printf("ans : %d\n",ans);
        for(int i=0;i<m;++i)
        {
            for(int j=0;j<n;++j)
            {

            }
        }
        for(int sz = min(m,n);sz>0;sz--)
        {
            //printf("%d\n",sz);
            cc[ans]=0;
            for(int i=0;i<m;++i)
            {
                for(int j=0;j<n;++j)
                {
                    if(ok(i,j,sz))
                    {
                        //printf("%d,%d : %d %d\n",i,j,sz,ans);
                        cc[ans]++;
                    }
                }

            }
            if(cc[ans])
            {
                aa[ans]=sz;
                ans++;
            }
        }
        printf("Case #%d: %d\n",Test++,ans);
        for(int i=0;i<ans;++i)printf("%d %d\n",aa[i],cc[i]);
    }
    return 0;
}
