#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#define Q 200

using namespace std;

int n,p,q;
int rel[Q];
int delky[Q];
int zlom[Q];
int nej,indexnej,akt;
int uspora,pocet;
int vysl;

int main()
{
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
	{
		for(int j=0;j<Q;j++) zlom[j]=0;
		
		scanf("%d%d",&p,&q);
		for(int j=0;j<q;j++) scanf("%d",rel+j);
		sort(rel,rel+q);
		vysl=(p-1)*q;
		
		delky[0]=rel[0]-1;
		for(int j=1;j<q;j++) delky[j]=rel[j]-rel[j-1]-1;
		delky[q]=p-rel[q-1];
		//for(int j=0;j<=q;j++) printf("%d ",delky[j]);
		//printf("\n");
		
		for(int u=0;u<q;u++)
		{	
			indexnej = -1;
			nej = 0;
			for(int j=0;j<q;j++)
			{
				if(zlom[j]) continue;
				akt=0;
				
				uspora=0;
				for(int k=j;k>=0;k--)
				{
					if(zlom[k]) break;
					uspora += delky[k]+1;
				}
				//uspora--;
				pocet=0;
				for(int k=j+1;k<q;k++)
				{
					if(zlom[k]) break;
					pocet++;
				}
				akt+=pocet*uspora;
				//printf("pro j=%d nalevo pocet=%d uspora=%d\n",j,pocet,uspora);
				
				uspora=0;
				for(int k=j;k<q;k++)
				{
					if(zlom[k]) break;
					uspora += delky[k+1]+1;
				}
				//uspora--;
				pocet=0;
				for(int k=j-1;k>=0;k--)
				{
					if(zlom[k]) break;
					pocet++;
				}
				akt+=pocet*uspora;
				//printf("pro j=%d napr pocet=%d uspora=%d\n",j,pocet,uspora);
				
				if(indexnej == -1 || akt > nej )
				{
					indexnej=j;
					nej=akt;
				}
			}
			zlom[indexnej]=1;
			vysl -= nej;
			//printf("index=%d nej=%d\n",indexnej,nej);
		}
		printf("Case #%d: %d\n",i,vysl);
	}
	return 0;
}
