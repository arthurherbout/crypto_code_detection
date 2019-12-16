#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char * decBin(int n) {
	int inc=1;
	char *bin = (char*) malloc((inc+1) * sizeof(char));
	int i=0;
	while(n>0) {
		int r = n%2;
		char c = r + '0';
		bin[i]=c;
		i++;
		n/=2;
		inc+=1;
		bin=(char*) realloc(bin,(inc+1));
	}
    char *binRev = (char*) malloc(sizeof(char) * (inc));
    for(int in=0;in<inc-1;in++) {
        char c = bin[in];
        binRev[inc-2-in]=c;
    }
	return binRev;
}

void main() {
	
	/*variable definitions*/
	int s[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
	int K[64];
	int i;
	printf("input value to convert\n");
	int input;
	scanf("%d",&input);
	char *result = decBin(input);
	printf("result is %s",result);
	exit(0);
}

