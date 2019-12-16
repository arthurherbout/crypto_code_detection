#include "generate.h"
#include "utils.h"

// Generate next permutation of current string
int generate_next_permutation(const char* end, char* curr, char* next) {
	int m = strlen(end);

	if (strcmp(end, curr)) {
		// Not end, generate next permutation
		// Find max j from right to left that s[j] < s[j+1]
		int j = m - 2;

		while (curr[j] >= curr[j + 1] && j > 0) {
			j--;
		}
		
		// Find k that k > j, s[k] > s[j] and s[k] <= s[i] with all i > k
		int k = j + 1;
		int i;
		for (i = k + 1; i < m; i++) {
			if (curr[i] > curr[j] && 
					curr[i] < curr[k]) {
				k = i;
			}
		}
		
		// Swap j, k
		strcpy(next, curr);
		swap(next, j, k);

		// Rotate the elements in the right of j
		rotate(next, j + 1, m - 1);

		printf("%s\n", next);
	} else {
		// End
		return -1;
	}
	return 1;
}

// Generate next subset
int generate_next_subset(int size, char* alphabet, char* curr, char* next) {

	return 1;
}

// Generate next array of indices of length k from n characters
int generate_next_indices(int size, int end[], int curr[], int next[]) {
	// Check end condition
	int stop = 1;
	int i;
	for (i = 0; i < size; i++) {
		if (curr[i] != end[i]) {
			stop = 0;
		}
	}

	// end
	if (stop) {
		return -1;
	}

	// Next string
	// Copy array
	for (i = 0; i < size; i++) {
		next[i] = curr[i];
	}

	// Recursive
	next_indices(next, size - 1);

	// Print result
	char str[size];

	indices_to_string(size, next, str);
	// printf("%s\n", str);
	// for (int i = 0; i < size; i++) {
	// 	printf("%c ", str[i]);
	// }
	// printf("\n");

	return 1;
}

// Next array of indices recursive
void next_indices(int next[], int i) {
	if (next[i] < N - 1) {
		next[i] += 1;
		return;
	} else {
		next[i] = 0;
		next_indices(next, i - 1);
	}
}

// Convert from indices array to string via alphabet
int indices_to_string(int size, int indices[], char* str) {
	int i;
	for (i = 0; i < size; i++) {
		str[i] = ALPHABET[indices[i]];
	}
	str[size] = '\0';
}


// int main(int argc, char** argv) {
// 	const int k = 5;
// 	char next[k];
// 	int curr_indices[] = {4, 5, 5, 23, 2};
// 	int next_indices[k];
// 	int end_indices[] = {4, 6, 2, 0, 2};
// 	int x;

// 	x = generate_next_indices(k, end_indices, curr_indices, next_indices);
	
// 	while (x != -1) {
// 		// Array copy
// 		for (int i = 0; i < k; i++) {
// 			curr_indices[i] = next_indices[i];
// 		}
// 		x = generate_next_indices(k, end_indices, curr_indices, next_indices);
// 	}
// 	return 1;	
// }
