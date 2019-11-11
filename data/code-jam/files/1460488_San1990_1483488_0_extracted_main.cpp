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

FILE * inputFile = fopen("D:\\Downloads\\Chrome\\C-small-attempt0.in", "r");
FILE * outputFile  = fopen("D:\\Downloads\\Chrome\\output.txt", "w");

int A,B,L;
int * toNumbers(const int &a)
{
	F(i,7) if (a/pow(10.0,7-i) >= 1) {L=7-i+1; break;}
	int * res = new int[L];
	int v = a;
	F(i,L)	
	{res[i] = v % 10; v /= 10;}
	return res;
}
int replace(int *a){
	int res = 0, p = 10;
	F(i,L-1) { res += (p*a[i]); p*=10;}
	res += a[L-1];
	return res;
}
int fillArray()
{
	int num = 0;
	FIAN(i,A,B){
		int *a = toNumbers(i);
		F(j,L-1) {
			int replaced = replace(a);
			if ((replaced <= B)&&(replaced >= A)&&(replaced!=i)) 
				num++;
		}
	}
    return num/2;
}

void oneStep()
{
    fscanf(inputFile, "%d %d", &A, &B);
    fprintf(outputFile, "%d\n", fillArray());
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