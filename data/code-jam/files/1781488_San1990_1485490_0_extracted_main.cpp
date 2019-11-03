#include <string>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <limits.h>

typedef unsigned __int64 lint; 

#define F(i,b) for(lint i=0;i<(b);i++)
#define FIAN(i,a,n) for(lint i=a;i<n;i++)
#define FIANrev(i,a,n) for(lint i=a;i>n;i--)

using namespace std;

FILE * inputFile = fopen("D:\\Downloads\\Chrome\\C-small-attempt1.in", "r");
FILE * outputFile  = fopen("D:\\Downloads\\Chrome\\output.txt", "w");

lint N,M;
lint *a,*A,*b,*B;

lint calcToysByIterator(const lint &ii, const lint &jj){
	
	lint i = ii, j = jj;
	if (i==2 && j==2)
		lint r = 0;

	if ((i==N)||(j==M)) return 0;
	lint res = 0;
	if (A[i]==B[j]){
		if (a[i] > b[j]){
			a[i] -= b[j];
			res += b[j] + calcToysByIterator(i,j+1);
			a[i] += b[j];
		}
		if (b[j] > a[i]){
			b[j] -= a[i];
			res += a[i] + calcToysByIterator(i+1,j);
			b[j] += a[i];
		}
		if (b[j] == a[i]){
			res += a[i] + calcToysByIterator(i+1,j+1);
			}
		}
	else{
		res += max(calcToysByIterator(i+1,j),calcToysByIterator(i,j+1));
	}
	return res;
}

void oneStep()
{
    fscanf(inputFile, "%I64d%I64d", &N, &M);

	a = new lint[N];
	A = new lint[N];
	b = new lint[M];
	B = new lint[M];

	F(i,N)	fscanf(inputFile, "%I64d%I64d", &a[i], &A[i]);
	F(i,M)	fscanf(inputFile, "%I64d%I64d", &b[i], &B[i]);
	
	lint r = calcToysByIterator(0,0);
	fprintf(outputFile, "%I64d ", r);
	fprintf(outputFile, "\n");
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