#include <stdio.h>

int a[10000], b[10000];
 
int main(void) 
{
	FILE *in = fopen("./input.txt", "r");
	FILE *out = fopen("./output.txt", "w");
	int loop;
	int count = 0;
        int num;
        int i, j, temp;
        unsigned long long intersection;
	fscanf(in, "%d", &count);
	for (loop = 1;loop<=count;loop++) {
            intersection = 0;
	    fscanf(in, "%d", &num);
            for (i = 0; i < num ; i++) {
                fscanf(in, "%d %d", &a[i], &b[i]);
            }
            for (i=0;i<num-1;i++) {
                for(j=0;j<num-i-1;j++) {
                    if(a[j] > a[j+1]) {
                       temp=a[j];
                       a[j]=a[j+1];
                       a[j+1]=temp;
                       temp=b[j];
                       b[j]=b[j+1];
                       b[j+1]=temp;                       
                    }
                }
            }  
            for (i=0;i<num;i++) {
                 for(j=i+1;j<num;j++) {
                     if(b[i] > b[j]) {
                        intersection++;
                     }
                  }
            }
            fprintf(out, "Case #%d: %llu\n", loop, intersection); 
	}
	fclose(in);
	fclose(out);

        return 0;
}
