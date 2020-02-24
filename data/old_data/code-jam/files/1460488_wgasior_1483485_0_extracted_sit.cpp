#include <cstdio>
#include <iostream>

using namespace std ;

char best_mapping_possible[] = {'y','h','e','s','o','c','v','x','d','u','i','g','l','b','k','r','z',
																't','n','w','j','p','f','m','a','q'} ;



int main() {

	int number_of_tests = 0 ;
	int case_number = 1 ; 
	char input_line[256] ;
	int current_char = 0 ;

	scanf("%d\n",&number_of_tests) ;
	while (case_number <= number_of_tests) {
		cin.getline(input_line, 256) ;
		current_char = 0 ;
		while (input_line[current_char] != '\0') {
			if (input_line[current_char] != ' ') input_line[current_char] = best_mapping_possible[input_line[current_char]-'a'] ;
			++current_char ;
		}

		printf("Case #%d: %s\n", case_number, input_line) ;
		++case_number ;
	}

	
	return 0 ;
}
