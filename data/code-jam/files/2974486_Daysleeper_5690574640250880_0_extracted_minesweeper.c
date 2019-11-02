#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int fill_neighbors_row(int Y, int counter, char ** miner) {
	int i;
	miner[0][0]='c';
	for (i=1;i<counter;i++)
		miner[0][i]='.';		
}

int fill_neighbors_column(int X, int counter, char ** miner) {
	int i;
	miner[0][0]='c';
	for (i=1;i<counter;i++)
		miner[i][0]='.';

}

int compute_grid(int X, int Y, int counter, int * x, int * y) {
	int impossible=0;
	(*x)=2;	
	(*y)=counter/(*x);
	while ((*x)>X || (*y+1)>Y) {
		(*x)++;
		(*y)=counter/(*x);
	}
	if ((*x)*(*y)<counter) { 
		(*y)++;
		if ((*y)==1)
			impossible=1;
		if (Y==2)
			impossible=1;
	}
	return impossible;
}

int fill_neighbors(int X, int Y, int counter, char ** miner) {
	int x,y,impossible=0;
	impossible=compute_grid(X,Y,counter,&x,&y);
	int i,j;
	if (counter==5)
		impossible=1;

	for (i=0;i<x;i++)
		for (j=0;j<y;j++)
			if (counter>0) {
				miner[i][j]='.';
				counter--;
			}
	miner[0][0]='c';
	if (miner[x-1][0]=='.' && miner[x-1][1]=='*')
		impossible=1;
	return impossible;
	
}

void special_case_M0(int X, int Y, char ** miner) {
	int i,j;
	for (i=0;i<X;i++)
		for (j=0;j<Y;j++)
			miner[i][j]='.';
	miner[0][0]='c';
}

void printer(int X, int Y, char ** miner) {
	int i,j;
	for (i=0;i<X;i++) {
		for (j=0;j<Y;j++)
			printf("%c",miner[i][j]);
		printf("\n");
	}	
}

void print_miner(int X,int Y,int M) {
	int i,j,impossible=0;
	int counter=X*Y-M;
	char ** miner=malloc(X*sizeof(char*));
	for (i=0;i<X;i++) {
		miner[i]=malloc(Y*sizeof(char));
		for (j=0;j<Y;j++)
			miner[i][j]='*';
	}
	if (M==0) 
		special_case_M0(X,Y,miner);
	else if (X==1) 
		fill_neighbors_row(Y,counter,miner);
	else if (Y==1)
		fill_neighbors_column(X,counter,miner);
	else 
		impossible=fill_neighbors(X,Y,counter,miner);

	if (impossible==0)
		printer(X,Y,miner);
	else
		printf("Impossible\n");
	for (i=0;i<X;i++)
		free(miner[i]);
	free(miner);
}
int main() {
	int cases,X,Y,M;
	int k,flag;
	scanf("%d",&cases);
	for (k=1;k<=cases;k++) {
		flag=0;
		scanf("%d %d %d",&X,&Y,&M);

		if (M!=0) {
			if (X==1 || Y==1 && X*Y-M<2) 
				flag=1;
			else if (X>1 && Y>1 && X*Y-M<4)
				flag=1;
		}
		if (flag) {
			printf("Case #%d:\nImpossible\n",k);	
		}
		else {
			printf("Case #%d:\n",k);
			print_miner(X,Y,M);
			

		}
	}
	
	return 0;
}
