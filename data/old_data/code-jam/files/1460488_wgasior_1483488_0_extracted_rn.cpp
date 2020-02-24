#include <cstdio>

int ok[10000] = {0} ;

int main() {

	int number_of_test_cases = 0 ;
	int current_tc = 1 ;
	int a_value = 0 ;
	int b_value = 0 ;
	int result = 0 ;
	int small_result = 0 ;
	int current_number = 0 ;
	int cv_cpy = 0 ;
	int rotated = 0 ;
	int rest = 0 ;
	int base = 0 ;
	int number_of_digits = 0 ;
	int multiplier = 1 ;


	scanf("%d", &number_of_test_cases) ;

	while(current_tc <= number_of_test_cases) {
		scanf("%d %d", &a_value, &b_value) ;
		result = 0 ;
		for (current_number = a_value ; current_number <= b_value ; ++current_number) {
			if (ok[current_number] == current_tc) continue ;
			number_of_digits = 1 ;
			cv_cpy = current_number ;
			small_result = 0 ;
			multiplier = 1 ;
			while (cv_cpy/10) {
				cv_cpy /= 10 ;
				multiplier *= 10 ;
				++number_of_digits ;
			}
			cv_cpy = current_number ;
			rotated = current_number ;
			while (cv_cpy/10) {
				rest = rotated % 10 ;
				base = rotated / 10 ;
				rest *= multiplier ;
				rotated = base + rest ;
				if (rotated == current_number) break ;
				if (rotated >= a_value && rotated <= b_value) {
					++small_result ;
					ok[rotated] = current_tc ;
				}
				cv_cpy /=10 ;
			}
			if (small_result) {
				if (small_result < 2) ++result ;
				else result += 3 ;
			}
									
		}


		printf("Case #%d: %d\n", current_tc, result) ;
		++current_tc ;
	}

	return 0 ;
}
