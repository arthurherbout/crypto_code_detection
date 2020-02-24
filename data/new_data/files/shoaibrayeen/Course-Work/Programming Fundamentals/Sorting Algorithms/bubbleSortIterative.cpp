#include <iostream>
using namespace std ;

class sort {
	private :
		int array[10] , number ;
	public :
		sort() ;
		void input() ;
		void display() ; 
		void bubble() ;
};

sort :: sort () {
	for ( int i = 0 ; i < 10 ; ++i )
		array[i] = 0;
	number = 0 ;
}

void sort :: input() {
	cout << "\nEnter number of elemnets\t:\t" ;
	cin >> number ;
	cout << "\nEnter Elements\n" ;
	for ( int i = 0 ; i < number ; ++i ) {
		cout << "\nEnter " << ( i + 1 ) << " element\t:\t" ;
		cin >> array[i] ;
		cout << endl ;
	}
}

void sort :: bubble() {
	for ( int i = number ; i > 0 ; --i ) {
		for ( int j = 1 ; j < number ; ++j ) {
			if ( array[j-1] > array[j] ) {
				int local = array[j-1] ;
				array[j-1] = array[j] ;
				array[j] = local ;
			} 
		}
		cout << "\nIterations\t\t:\t" ;
		display();
	}
}

void sort :: display() {
	for ( int i = 0 ; i < number ; ++i ) {
		cout << array[i] <<"\t" ;
	}
	cout << endl ;
}

int main() {
	sort object;
	object.input() ;
	cout << "\nEntered Elements\t:\t" ;
	object.display() ;
	object.bubble() ;
	cout << "\nSorted Elements \t:\t" ;
	object.display() ;
	return 0;
}
