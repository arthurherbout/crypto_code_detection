/*
 * Angel Sancho <angelitodeb@gmail.com>
 *
 * Google Code Jam 2012 - Qualification Round - C. Recycled Numbers
 *
 * Big version
 */

#include <stdio.h>
#include <string.h>

int __pow(int b, int e) {
	int i, ret = b;

	for(i=1;i<e;i++) {
		ret *= b;
	}

	return ret;
}

void main(int argc, char **argv) {
	FILE *fp;
	int T,c, ok;
	unsigned int j, k, len, d, r;
	unsigned long num, i, min, max, total;
	unsigned long ant[8];
	char buff[8];

	fp = fopen(argv[1], "r");

	fscanf(fp, "%d", &T);

	for(c=1;c<=T;c++) {
		fscanf(fp, "%lu %lu", &min, &max);

		total = 0;

		for(i=min;i<=max;i++) {
			sprintf(buff, "%lu", i);
			len = strlen(buff);
	
			for(k=0;k<8;k++) {
				ant[k] = 0;
			}
	
			for(j=1;j<len;j++) {
				r = __pow(10, j);
				d = __pow(10, (len-j));
		
				num = ((i%r)*d)+(int)(i/r);
	
				if(num >= min && num <= max && i != num) {
					if(num > i) {
						ok = 1;
	
						for(k=0;k<(j-1);k++) {
							if(ant[k] == num) {
								ok = 0;
							}
						}
	
						if(ok == 1) {
							ant[j-1] = num;
							total++;
						}
					}
				}
			}
		}
	
		printf("Case #%d: %lu\n", c, total);
	}

	fclose(fp);
}
