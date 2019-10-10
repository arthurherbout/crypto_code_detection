#include <cstdio>

#define MAX_CLASSES 1000

int main() {

	unsigned int neighbour_tabe[50][51] ;
	unsigned int start_points[50] ;
	unsigned int number_of_start_points = 0 ;
	unsigned int number_of_tc = 0 ;
	unsigned int current_tc = 0 ;
	unsigned int number_of_classes = 0 ;
	unsigned int current_class = 0 ;
	unsigned int counter = 0 ;
	unsigned int current_start_point = 0 ;
	unsigned int classes[MAX_CLASSES] ;
	unsigned int number_of_base_classes = 0;
	unsigned int class_number = 0 ;
	unsigned int path[MAX_CLASSES] ;
	unsigned int path_depth = 0 ;
	unsigned int counters[MAX_CLASSES] ;
	bool result = false ;


	scanf("%u", &number_of_tc) ;

	while (current_tc < number_of_tc) {
		++current_tc ;
		scanf("%u", &number_of_classes) ;
		number_of_start_points = 0 ;
		result = true ;
		current_class = 0 ;
		for (counter = 0 ; counter < number_of_classes ; ++counter) neighbour_tabe[counter][0] = 0 ;

		while (current_class < number_of_classes) {
			scanf("%u",&number_of_base_classes) ;
			if (number_of_base_classes == 0) {
				start_points[number_of_start_points] = current_class ;
				++number_of_start_points ;
			}
			counter = 0 ;
			while (counter < number_of_base_classes) {
				++counter ;
				scanf("%u", &class_number) ;
				--class_number ;
				neighbour_tabe[class_number][neighbour_tabe[class_number][0]+1] = current_class ;
				++neighbour_tabe[class_number][0] ;
			}
			++current_class ;
		}

		current_start_point = 0 ;
		for (counter = 0 ; counter < MAX_CLASSES ; ++counter) classes[counter] = 0 ;

		while (current_start_point < number_of_start_points) {
			path_depth = 0 ;
			path[0] = start_points[current_start_point] ;
			class_number = neighbour_tabe[path[0]][0] ;
			counters[0] = 0 ;

			while(1) {
				if (counters[path_depth] <= neighbour_tabe[path[path_depth]][0]) {
					if (classes[path[path_depth]] == current_start_point+1 && counters[path_depth] ==0) {
						result = false ;
						break ;
					} else classes[path[path_depth]] = current_start_point+1 ;
					if (counters[path_depth] < neighbour_tabe[path[path_depth]][0]) {
						path[path_depth+1] = neighbour_tabe[path[path_depth]][counters[path_depth]+1] ;
						++path_depth ;
						counters[path_depth] = 0 ;
					} else ++counters[path_depth] ;
				} else {
					if (path_depth == 0) {
						break ;
					}
					--path_depth ;
					++counters[path_depth] ;
				}
			}
			++current_start_point ;
		}

		printf("Case #%u: ", current_tc) ;
		if (!result) printf("Yes\n") ;
		else printf("No\n") ;


	}

	

	return 0 ;
}
