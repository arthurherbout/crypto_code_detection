#include <cstdio>


int main() {

	int number_of_tt =  0 ;
	int case_number = 1 ;
	int number_of_googlers = 0 ;
	int number_of_st = 0 ;
	int min_points = 0 ;
	int result = 0 ;
	int base = 0 ;
	int div_rest = 0 ;
	int current_points = 0 ;
	scanf("%d", &number_of_tt) ;

	while (case_number <= number_of_tt) {
		result = 0 ;
		scanf("%d %d %d", &number_of_googlers, &number_of_st, &min_points) ;
		while (number_of_googlers) {
			scanf("%d", &current_points) ;
			base = current_points / 3 ;
			div_rest = current_points % 3 ;
			if (base >= min_points ) {
				++result ;
			} else if (base == min_points-1 && base)  {
				if (div_rest == 0 ) {
					if (number_of_st) {
						++result ;
						--number_of_st ;
						}
				} else ++result ;
			}	else if (base == min_points-2 && div_rest == 2 && number_of_st > 0) {
				++result ;
				--number_of_st ;
			}

			--number_of_googlers ;
		}
		printf("Case #%d: %d\n", case_number, result) ; 
		++case_number ;
	}


	return 0 ;
}
