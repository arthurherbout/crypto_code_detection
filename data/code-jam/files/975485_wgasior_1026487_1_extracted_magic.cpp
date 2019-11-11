#include <stdio.h>
#include <map>
#include <string.h>
#include <list>

class Opposed {

	public :
		Opposed(): size_('Z'-'A'+1), begin_('A') {
			Init() ;
		}
		Opposed(int size, int begin): size_(size), begin_(begin) {
			Init() ;
		}
		~Opposed() {
			delete [] opposed_symbol_ ;
		}
		void AddOpposedPair(char symbol1, char symbol2) {
			opposed_symbol_[symbol1-begin_].insert( std::pair<char, char>(symbol2, symbol1) ) ;
			opposed_symbol_[symbol2-begin_].insert( std::pair<char, char>(symbol1, symbol2) ) ;						
		}

		inline std::map<char,char>* GetOpposedSymbols(char symbol) {
			return &opposed_symbol_[symbol-begin_] ;
		}

		void Reset() {
			for (int counter = 0 ; counter < size_ ; ++counter) {
				opposed_symbol_[counter].clear() ;				
			}
		}

	private :
		void Init() {
			opposed_symbol_ = new std::map<char,char> [size_] ; 
		}
		int size_ ;
		int begin_ ;
		std::map<char, char> *opposed_symbol_ ;
} ;

class SymbolProduct {
	public :
		SymbolProduct() : size_('Z'-'A'+1) {
			Init() ;
		}

		SymbolProduct(int size): size_(size) {
			Init() ;
		}

		void AddProduction(char symbol1, char symbol2, char product) {
			int position1 = symbol1-'A' ;
			int position2 = symbol2-'A' ;
			product_table_[size_*position1 + position2] = product ;
			product_table_[size_*position2 + position1] = product ;
		}

		char GetProduction(char symbol1, char symbol2) {
			return product_table_[(symbol1-'A')*size_+(symbol2-'A')] ;
		}

		void Reset() {
			memset(product_table_, 0, size_*size_) ;
		}

	private :
		void Init() {
			product_table_ = new char[size_*size_] ;
			memset(product_table_, 0, size_*size_) ;
		}
		char* product_table_ ;
		int size_ ;
} ;

class MagicSymbols {
	public:

	MagicSymbols(): size_('Z'-'A'+1) {
		blocked_symbols_ = new char[size_] ;
		memset(blocked_symbols_, 0, size_) ;
	}

	~MagicSymbols() {}

	void AddOpposedPair(char symbol1, char symbol2) {
		opposed_symbols_.AddOpposedPair(symbol1, symbol2) ;
	}

	void AddProduction(char symbol1, char symbol2, char product) {
		symbol_product_.AddProduction(symbol1, symbol2, product) ;
	}

	void AddSymbols(char* symbols, int length) {
		int number_of_symbols = 'Z'-'A' + 1 ;
		int current_symbol_number = 0 ;
		char current_symbol = symbols[0] ; 
		

		while (current_symbol_number < length) {
			current_symbol = symbols[current_symbol_number] ;
			if (elements_list_.size() == 0) {
				elements_list_.push_back(current_symbol) ;
				UpdateBlockedSymbols(current_symbol, true) ;
				++current_symbol_number ;
			}
			else {

				while (symbol_product_.GetProduction(elements_list_.back(), 
																						 current_symbol) > 0 &&
								elements_list_.size() > 0) {
					current_symbol = symbol_product_.GetProduction(elements_list_.back(),
																												current_symbol) ;

					UpdateBlockedSymbols(elements_list_.back(), false) ;
					elements_list_.pop_back() ;
				}
				if (blocked_symbols_[current_symbol-'A'] > 0) {
					ClearElementsList() ;
				} else {
					elements_list_.push_back(current_symbol) ;
					UpdateBlockedSymbols(current_symbol, true) ;
				}
							
				++current_symbol_number ;
			}
		}

	}
		void ClearElementsList() {
			memset(blocked_symbols_, 0, size_) ;
			elements_list_.clear() ;
		}
		std::list<char>* GetResult() {
			return &elements_list_ ;
		}

		void Reset() {
			memset(blocked_symbols_, 0, size_) ;
			elements_list_.clear() ;
			opposed_symbols_.Reset() ;
			symbol_product_.Reset() ;
		}

	private:
		void UpdateBlockedSymbols(char symbol, bool add_symbol){
			int change_value = 0 ;
			const std::map<char,char>* blocked_symbols ;
			if (add_symbol) change_value = 1 ;
			else change_value = -1 ;
			blocked_symbols = opposed_symbols_.GetOpposedSymbols(symbol) ;
			for (std::map<char,char>::const_iterator iter = blocked_symbols->begin() ; 
					iter != blocked_symbols->end() ; iter++) {
					blocked_symbols_[(*iter).first - 'A'] += change_value ;
			}
		}
		Opposed opposed_symbols_ ;
		SymbolProduct symbol_product_ ;
		char* blocked_symbols_ ;
		std::list<char> elements_list_ ;
		int size_ ;
} ;

int main(int argl, char** args) {

	int number_of_cases = 0 ;
	int current_case = 1 ;
	int number_of_strings = 0 ;
	char current_string[101] ;
	MagicSymbols magic_symbols ;
	std::list<char>* result ;

	scanf("%d", &number_of_cases) ;

	while (current_case <= number_of_cases) {

		scanf("%d", &number_of_strings) ;
		while (number_of_strings > 0) {
			scanf("%s", current_string) ;
			magic_symbols.AddProduction(current_string[0], current_string[1], current_string[2]) ;
			--number_of_strings ;
		}

		scanf("%d", &number_of_strings) ;
		while (number_of_strings > 0) {
			scanf("%s", current_string) ;
			magic_symbols.AddOpposedPair(current_string[0], current_string[1]) ;
			--number_of_strings ;
		}

		scanf("%d", &number_of_strings) ;
		scanf("%s", current_string) ;

		magic_symbols.AddSymbols(current_string, number_of_strings) ;
		
		result = magic_symbols.GetResult() ;

		printf("Case #%d: [", current_case) ;

		for (std::list<char>::iterator iter = result->begin() ; 
					iter != result->end() ;
					iter++ ) {
					if (iter != result->begin()) printf(", ") ;
					printf("%c", *iter) ;
		}

		printf("]\n") ;
		magic_symbols.Reset() ;
		
		++current_case ;
	}

	return 0 ;
}
