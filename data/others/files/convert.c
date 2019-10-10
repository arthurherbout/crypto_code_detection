#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	short a; char b;
#ifdef DOWNSAMPLE
	while(fread(&a, 2, 1, stdin) == 1) {
		b = a / 0x100;
		fwrite(&b, 1, 1, stdout);
	}
#else
	while(fread(&b, 1, 1, stdin) == 1) {
		a = b * 0x100;
		fwrite(&a, 2, 1, stdout);
	}
#endif
	return 0;
	
}
