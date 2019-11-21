#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a[4][4]={{0}};
    int b[4][4]={{0}};
    int t,ii,i,j,fir,sec,flag=0,val;
    scanf("%d",&t);
    FILE *fp;
    fp=fopen("/home/mridul/CFiles/gji/output.txt", "w");
    for(ii=1; ii <= t; ii++)
    {
        val=0;
        flag=0;
        scanf("%d",&fir);
        for(i=0; i < 4; i++)
            scanf("%d %d %d %d",&a[i][0],&a[i][1],&a[i][2],&a[i][3]);
        scanf("%d",&sec);
        for(i=0; i < 4; i++)
            scanf("%d %d %d %d",&b[i][0],&b[i][1],&b[i][2],&b[i][3]);

        for(i=0; i < 4; i++)
        {
            for(j=0; j < 4; j++)
                if(a[fir-1][i] == b[sec-1][j])
                {
                    flag++;
                    val = a[fir-1][i];
                    if(flag > 1)
                        break;
                }
            if(flag > 1)
                break;
         }

        if(flag == 1)
            fprintf(fp,"Case #%d: %d\n",ii,val);
        else if(flag > 1)
            fprintf(fp,"Case #%d: Bad magician!\n",ii);
        else
            fprintf(fp,"Case #%d: Volunteer cheated!\n",ii);
    }
    fclose(fp);
    return;
}
