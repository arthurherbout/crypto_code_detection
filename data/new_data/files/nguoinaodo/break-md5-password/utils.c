#include "utils.h"

// Swap characters in a string
void swap(char* str, int i, int j) {
	char t;

	t = str[i];
	str[i] = str[j];
	str[j] = t;
}

// Rotate string
void rotate(char* str, int start, int end) {
	while (start < end) {
		swap(str, start, end);
		start++;
		end--;
	}
}

/*
int main() {
	char a[] = {'2', '4', '1'};

	swap(a, 0, 2);
	for (int i = 0; i < 3; i++) {
		printf("a[%d]=%c", i, a[i]);
	}
}
*/