#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>
using namespace std;

int tcase,n,m,ansA,ansB;
char IN[110][110],MAP[110][110];

int GOAL,ORD[110];
bool oA[110],oB[110],oC[210],oD[210];
bool xA[110],xB[110],pC[210],pD[210];
bool ok;

void input()
{
    int i,x,y;
    char t[3];
    
    scanf("%d%d",&n,&m);
    memset(IN,0,sizeof(IN)); memset(MAP,0,sizeof(MAP));
    memset(oA,0,sizeof(oA)); memset(oB,0,sizeof(oB));
    memset(oC,0,sizeof(oC)); memset(oD,0,sizeof(oD));
    memset(xA,0,sizeof(xA)); memset(xB,0,sizeof(xB));
    memset(pC,0,sizeof(pC)); memset(pD,0,sizeof(pD));
    
    for(i=1;i<=m;i++)
    {
        scanf("%s%d%d",t,&x,&y);
        IN[x][y] = MAP[x][y] = t[0];
        
        if(t[0] == 'o')
            oA[x] = oB[y] = oC[x+y] = oD[x-y+n] = true;
        else if(t[0] == 'x')
            oA[x] = oB[y] = true;
        else
            oC[x+y] = oD[x-y+n] = true;
    }
    
    if(n == 2 || n == 3) GOAL = n-1;
    else GOAL = n;
    
    for(i=1;i<=n;i++) ORD[i] = i;
    random_shuffle(ORD+1,ORD+1+n);
}

void dfs(int row,int sum)
{
    int i,j;
    char old;
    
    if(row > n)
    {
        ok = true;
        return;
    }
    for(j=1;j<=n;j++) if(MAP[row][j] == 'o')
    {
        dfs(row+1,sum+1);
        return;
    }
    
    for(i=1;i<=n;i++)
    {
        j = ORD[i];
        old = MAP[row][j];
        if(old == 'x')
            oA[row] = oB[j] = false;
        else if(old == '+')
            oC[row+j] = oD[row-j+n] = false;

        if(!oA[row] && !oB[j] && !oC[row+j] && !oD[row-j+n])
        {
            MAP[row][j] = 'o';
            oA[row] = oB[j] = oC[row+j] = oD[row-j+n] = true;
            dfs(row+1,sum+1);
            if(ok) return;
            oA[row] = oB[j] = oC[row+j] = oD[row-j+n] = false;
        }
        
        if(old == 'x')
            oA[row] = oB[j] = true;
        else if(old == '+')
            oC[row+j] = oD[row-j+n] = true;
        MAP[row][j] = old;
    }
    
    dfs(row+1,sum);
}

void placeXP()
{
    int i,j;
    
    for(i=1;i<=n;i++) for(j=1;j<=n;j++)
    {
        if(MAP[i][j] == 'x' || MAP[i][j] == 'o')
            xA[i] = xB[j] = true;
        else if(MAP[i][j] == '+' || MAP[i][j] == 'o')
            pC[i+j] = pD[i-j+n] = true;
    }
    
    for(i=1;i<=n;i++) for(j=1;j<=n;j++) if(!MAP[i][j])
    {
        if(xA[i] || xB[j]) continue;
        MAP[i][j] = 'x';
        xA[i] = xB[j] = true;
    }
    
    for(i=1;i<=n;i++) for(j=1;j<=n;j++) if(!MAP[i][j])
    {
        if(pC[i+j] || pD[i-j+n]) continue;
        MAP[i][j] = '+';
        pC[i+j] = pD[i-j+n] = true;
    }
}

void printAns()
{
    int i,j;
    ansA = ansB = 0;
    
    for(i=1;i<=n;i++) for(j=1;j<=n;j++)
    {
        if(MAP[i][j] == 'x' || MAP[i][j] == '+') ansA++;
        else if(MAP[i][j] == 'o') ansA += 2;
        if(MAP[i][j] != IN[i][j]) ansB++;
    }
    
    printf("%d %d\n",ansA,ansB);
    for(i=1;i<=n;i++) for(j=1;j<=n;j++) if(MAP[i][j] != IN[i][j])
        printf("%c %d %d\n",MAP[i][j],i,j);
}

int main()
{
    int cas;
    
    srand(time(0));
    scanf("%d",&tcase);
    for(cas=1;cas<=tcase;cas++)
    {
        input();
        ok = false; dfs(1,0);
        placeXP();
        printf("Case #%d: ",cas);
        printAns();
    }
    
    return 0;
}