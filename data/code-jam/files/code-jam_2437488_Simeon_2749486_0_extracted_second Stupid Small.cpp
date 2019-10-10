#include <fstream>
using namespace std;
typedef long long lld;

bool MoveUpDown (lld num)
{
    if (num<0)
    {
        printf("NS");

    }
    else if (num>0)
    {
        printf("SN");
    }
}
bool MoveLeftRight(lld num)
{
    if (num<0)
    {
        printf("EW");
    }
    else if (num>0)
    {
        printf("WE");
    }
    else
    {
        return false;
    }
    return true;
}
int main ()
{
    freopen("input.txt","r",stdin);
    freopen ("output.txt","w",stdout);
    lld i,j,ii,jj,ind;
    lld tests,t;
    lld x,y,posx,posy;

    scanf("%I64d",&tests);
    for (t=1;t<=tests;t++)
    {
        scanf("%I64d %I64d",&x,&y);
        posx=0;
        posy=0;
        printf("Case #%I64d: ",t);
        while (MoveUpDown(y-posy))
        {
            if (y-posy>0) y--;
            else y++;
        }
        while (MoveLeftRight(x-posx))
        {
            if (x-posx>0) x--;
            else x++;
        }
        printf("\n");
    }
}
