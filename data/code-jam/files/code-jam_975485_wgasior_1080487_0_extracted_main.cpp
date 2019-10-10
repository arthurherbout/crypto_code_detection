#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

class Robot {
	private:
		int position_ ;
		int time_to_perform_tasks_ ;
		char symbol_ ;

	public:
		Robot(char robot_symbol) : position_(1), 
																time_to_perform_tasks_(0), 
																symbol_(robot_symbol) {
		}
		~Robot() {}
		inline char GetSymbol() {return symbol_ ;}
		inline int GetUsedTime() {return time_to_perform_tasks_ ;}
		void PressButton(int next_position, int time_buffer) {
			time_to_perform_tasks_ += (std::max(0, abs(position_ - next_position)-time_buffer) + 1) ;
			position_ = next_position ;
		} 
		void ResetRobot() {
			position_ = 1 ;
			time_to_perform_tasks_ = 0 ;
		}
		void ResetTime() { time_to_perform_tasks_ = 0 ;}
} ;

int main(int argl, char** args) {
	
	Robot* current_robot = new Robot('O') ;
	Robot* next_robot = new Robot('B') ;
	Robot* temp_robot_pointer = NULL ;
	
	int number_of_test_cases = 0 ;
	int current_test_case = 1 ;
	int sequence_length = 0 ;
	
	char robot_symbol[2] ;
	int button_number = 0 ;

	int result = 0 ;


	scanf("%d", &number_of_test_cases) ;

	while (current_test_case <= number_of_test_cases) {
		result = 0 ;
		current_robot->ResetRobot() ;
		next_robot->ResetRobot() ;
		scanf("%d", &sequence_length) ;
		while (sequence_length > 0) {
			scanf("%s", robot_symbol) ;
			scanf("%d", &button_number) ;
			if (current_robot->GetSymbol() == robot_symbol[0]) {
				current_robot->PressButton(button_number, 0) ;
			} else {
				result += current_robot->GetUsedTime() ;
				next_robot->PressButton(button_number, current_robot->GetUsedTime()) ;
				current_robot->ResetTime() ;
				temp_robot_pointer = current_robot ; current_robot = next_robot ; 
				next_robot = temp_robot_pointer ;
			}
			--sequence_length ;
		}
		result += current_robot->GetUsedTime() ;
		printf("Case #%d: %d\n", current_test_case, result) ;
		++current_test_case ;
	}

	return 0 ;
}
