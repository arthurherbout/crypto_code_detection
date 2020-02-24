#include <stdio.h>


int main(int argl, char** args) {

	unsigned long result = 0 ;
	int number_of_test_cases = 0 ;
	int current_test_case = 1 ;
	int number_of_candies = 0 ;
	unsigned long current_candy = 0 ;
	unsigned long minimum_value = 0 ;
	unsigned long split_condition = 0 ;


	scanf("%d", &number_of_test_cases) ;
	while (current_test_case <= number_of_test_cases) {
		split_condition = 0 ;
		minimum_value = 0 ;
		result = 0 ;
		scanf("%d", &number_of_candies) ;
		while (number_of_candies > 0) {
			scanf("%lu", &current_candy) ;
			if (minimum_value == 0) minimum_value = current_candy ;
			if (current_candy < minimum_value) {
				minimum_value = current_candy ;
			}
			result += current_candy ;
			split_condition ^= current_candy ;
			--number_of_candies ;
		}
		printf("Case #%d: ", current_test_case) ;
		if (split_condition > 0) printf("NO\n") ;
		else printf("%lu\n", result-minimum_value) ;
		++current_test_case ;
	}

	return 0 ;
}
