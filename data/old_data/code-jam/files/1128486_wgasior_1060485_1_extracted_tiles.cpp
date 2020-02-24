#include <cstdio>


int main(int argl, char** args) {

	bool result = true ;
	int open_tiles = 0 ;
	int number_of_test_cases = 0 ;
	int current_test_case = 1 ;
	char picture[50][51] ;
	char closed_ties[50][51] ;
	unsigned int number_of_lines = 0 ;
	unsigned int number_of_characters = 0 ;
	unsigned int counter1 = 0 ;
	unsigned int counter2 = 0 ;


	scanf("%d", &number_of_test_cases) ;
	while (current_test_case <= number_of_test_cases) {
		scanf("%ud", &number_of_lines) ;
		scanf("%ud", &number_of_characters) ;
		counter1 = 0 ;
		open_tiles = 0 ;
		while (counter1 < number_of_lines) {
			scanf("%s", picture[counter1]) ;
			counter2 = 0 ;
			while (counter2 < number_of_characters) {
				closed_ties[counter1][counter2] = 0 ;
				if (picture[counter1][counter2] == '#') {
					if (counter1 > 0 && picture[counter1-1][counter2] == '/' && 
						!(counter2 > 0 && closed_ties[counter1-1][counter2-1] == 1) ) {
						picture[counter1][counter2] = '\\' ; 
						if (picture[counter1][counter2+1] == '#') {
							picture[counter1][counter2+1] = '/' ;
							--open_tiles ;
							closed_ties[counter1][counter2] = 1 ;
						} else break ;
					} else {
						picture[counter1][counter2] = '/' ;
						++open_tiles ;
						if (picture[counter1][counter2+1] == '#' ) {
							picture[counter1][counter2+1] = '\\' ;
						} else break ;
					}
				}
				++counter2 ;
			}		
			++counter1 ;
		}
		printf("Case #%d:\n", current_test_case) ;
		if (open_tiles != 0) printf("Impossible\n") ;
		else {
			counter1 = 0 ;
			while (counter1 < number_of_lines) {
				printf("%s\n", picture[counter1]) ;
				++counter1 ;
			}
		}
		++current_test_case ;
	}

	return 0 ;
}
