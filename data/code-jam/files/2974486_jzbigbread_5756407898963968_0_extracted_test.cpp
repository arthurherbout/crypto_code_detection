#include <stdio.h>
int a[6][6],b[6][6];
int ans,ansnum;
int n,m;
int main()
{
	int t;
	FILE *fin = fopen("D:\\1.in","r");
	FILE *fout = fopen("d:\\1.txt","w");
	fscanf(fin,"%d",&t);
	for (int k=1;k<=t;k++)
	{
		fscanf(fin,"%d",&n);
		for (int i=1;i<=4;i++)
			for (int j=1;j<=4;j++)
			{
				fscanf(fin,"%d",&a[i][j]);
			}
		fscanf(fin,"%d",&m);
		for (int i=1;i<=4;i++)
			for (int j=1;j<=4;j++)
			{
				fscanf(fin,"%d",&b[i][j]);
			}
		int ans = 0, ansnum = 0;
		for (int i=1;i<=4;i++)
			for (int j=1;j<=4;j++)
			{
				if (a[n][i]==b[m][j])
				{
					ansnum++;
					ans = a[n][i];
				}
			}
		if (ansnum<1)
			fprintf(fout,"Case #%d: Volunteer cheated!\n",k);
		else if(ansnum==1)
			fprintf(fout,"Case #%d: %d\n",k,ans);
		else fprintf(fout,"Case #%d: Bad magician!\n",k);
	}
}