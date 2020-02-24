#include<stdio.h>
#include<string.h>
FILE* fin;
FILE* fout;


int R[1005];
int X[1005];
int Y[1005];
int order[1005];
bool placed[1005];
int N, W, L;
int done = 0;
	
void fill2(int minX, int minY, int maxX, int maxY, int idx, bool fitX, bool shiftX, bool fitY, bool shiftY, bool limitY)
{
	//printf("->%d (%d %d %d %d)[%d]", idx, minX, maxX, minY, maxY, ((idx<N)?placed[order[idx]]:-1));
	if(idx>=N)
		return;
	if(placed[order[idx]])
	{
		fill2(minX, minY, maxX, maxY, idx+1, fitX, shiftX, fitY, shiftY, limitY);
		return;
	}
	bool proceedX = R[order[idx]]<maxX-minX;
	proceedX |= !fitX;
	bool proceedY = R[order[idx]]<maxY-minY && (!limitY || !shiftY);
	proceedY |= R[order[idx]]*2<maxY-minY;
	proceedY |= !fitY;
	
	if(!(proceedX && proceedY))
	{
		//printf(" no fit\n");
		fill2(minX, minY, maxX, maxY, idx+1, fitX, shiftX, fitY, shiftY, limitY);
	}
	else
	{
		//printf(" fit\n");
		if(shiftX)
			X[order[idx]]=minX+R[order[idx]];
		else
			X[order[idx]]=minX;
		if(shiftY)
			Y[order[idx]]=minY+R[order[idx]];
		else
			Y[order[idx]]=minY;
		done++;
		placed[order[idx]]=true;
		int w = (shiftX)?R[order[idx]]*2:R[order[idx]];
		int h = (shiftY)?R[order[idx]]*2:R[order[idx]];
		fill2(minX+w, minY, maxX, h, idx+1, true, true, true, shiftY, true);
		fill2(minX, minY+h, maxX, maxY, idx+1, fitX, shiftX, true, true, limitY);
	}
}

bool intersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	if(x1>=x4)
		return false;
	if(x3>=x2)
		return false;
	if(y1>=y4)
		return false;
	if(y3>=y2)
		return false;
	return true;	
}

int main(void)
{
	fin=fopen("aerobics.in", "r");
	fout=fopen("aerobics.out", "w");
	int T;
	fscanf(fin, "%d", &T);
	
	for(int tnum=1; tnum<=T; tnum++)
	{
		fprintf(fout, "Case #%d: ", tnum);
		fscanf(fin, "%d %d %d", &N, &W, &L);
		for(int i=0; i<N; i++)
		{
			placed[i]=false;
			fscanf(fin, "%d", &R[i]);
		}
		int max=999999999;
		for(int i=0; i<N; i++)
		{
			int idx=-1;
			for(int j=0; j<N; j++)
				if(R[j]<=max && !placed[j])
					if(idx==-1 || R[idx]<R[j])
						idx=j;
			order[i]=idx;
			placed[idx]=true;
			max=R[idx];
			X[i]=-1;
			Y[i]=-1;
		}
		for(int i=0; i<N; i++)
		{
			placed[i]=false;
		}
		bool swapped=false;
		if(W<L)
		{
			int temp = W;
			W=L;
			L=temp;
			swapped=true;
		}
		fill2(0,0,W,L,0, false, false, false, false, false);
		for(int i=0; i<N; i++)
			fprintf(fout, "%d.0 %d.0 ", swapped?Y[i]:X[i], swapped?X[i]:Y[i]);
		fprintf(fout, "\n");
		printf("%d\n", tnum);
		for(int i=0; i<N; i++)
			printf("%d ", (X[i]<=W && Y[i]<=L));
		printf("\n");
		for(int i=0; i<N; i++)
		{
			for(int j=0; j<N; j++)
				printf("%d ", (intersect(X[i]-R[i], Y[i]-R[i], X[i]+R[i], Y[i]+R[i], X[j]-R[j], Y[j]-R[j], X[j]+R[j], Y[j]+R[j])));
			printf("\n");
		}
		getchar();
	}
	
	return 0;
}
