#include <string>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>

#define F(i,b) for(int i=0;i<(b);i++)
#define FIAN(i,a,n) for(int i=a;i<n;i++)

using namespace std;

FILE * inputFile = fopen("D:\\Downloads\\Chrome\\B-small-attempt0.in", "r");
FILE * outputFile  = fopen("D:\\Downloads\\Chrome\\output.txt", "w");

int N,**ab;

void bubbleSort(int **arr,int length, int k)//Bubble sort function 
{
    int i,j;
    for(i=0;i<length;i++)
    {
        for(j=0;j<i;j++)
            if(arr[i][k]<arr[j][k])	{
					int *temp=arr[i]; //swap 
					arr[i]=arr[j];
					arr[j]=temp;	}
    }
}

void bubbleSortReverce(int **arr,int length, int k)//Bubble sort function 
{
    int i,j;
    for(i=0;i<length;i++)
    {
        for(j=0;j<i;j++)
            if(arr[i][k]>arr[j][k])	{
					int *temp=arr[i]; //swap 
					arr[i]=arr[j];
					arr[j]=temp;	}
    }
}

int fillArray()
{
	ab = new int*[N];
	F(i,N) { 
		ab[i] = new int[3]; ab[i][2] = 2;
		fscanf(inputFile, "%d %d", &ab[i][0], &ab[i][1]); 
	}

	bubbleSort(ab, N, 0);
	bubbleSortReverce(ab, N, 1);
	
	F(i,N) {printf(" %d %d \n", ab[i][0], ab[i][1]);}

	int num = 0, lvl = 0, curLvl = -1;
	for(;num < 2*N;){
		curLvl = lvl;
		F(i,N) {
			if (num >= ab[i][1]){
				ab[i][0] = 2000000;
				ab[i][1] = 2000000;
				num += ab[i][2];
				ab[i][2] = 0;
				lvl += 1;
			}
		}
		if (lvl==curLvl) {
			F(i,N) {
				if (num >= ab[i][0]){
					ab[i][0] = 2000000;
					num += 1;
					ab[i][2] = 1;
					lvl += 1;
					break;
				}
			}
			if (lvl==curLvl) return 0;
		}
	}
    return lvl;
}

void oneStep()
{
    fscanf(inputFile, "%d", &N);
	int tmp = fillArray();
	if (tmp>0) fprintf(outputFile, "%d\n", tmp);
	else fprintf(outputFile, "%s\n", "Too Bad");
}

int main()
{
    time_t timer = time(NULL);
    printf("%s\n", ctime(&timer));

    unsigned int T;
    fscanf(inputFile, "%d", &T);

    F(t,T){
        //TODO: solve here!
        fprintf(outputFile, "Case #%d: ", t+1);
        oneStep();
        timer = time(NULL);
        printf("step %d - %s\n", t, ctime(&timer));
    }

    fclose(inputFile);
    fclose(outputFile);

    timer = time(NULL);
    printf("%s\n", ctime(&timer));
   // qDebug("Time elapsed: %d ms", time1.elapsed());
    return 0;
}