#include<iostream>
#include<string>
#include<map>

class Googlerese;

class Googlerese{
public:
	static const int alphabet_size = 26;
	// int decode_map[26];
	std::map<char, char> decode_map;
	Googlerese() {
		for(int i = 0; i < alphabet_size; ++i) {
			decode_map[i] = i = 'a';
		}
		// decode_map[''] = '';
		decode_map['a'] = 'y';
		decode_map['b'] = 'h';
		decode_map['c'] = 'e';
		decode_map['d'] = 's';
		decode_map['e'] = 'o';
		decode_map['f'] = 'c';
		decode_map['g'] = 'v';
		decode_map['h'] = 'x';
		decode_map['i'] = 'd';
		decode_map['j'] = 'u';
		decode_map['k'] = 'i';
		decode_map['l'] = 'g';
		decode_map['m'] = 'l';
		decode_map['n'] = 'b';
		decode_map['o'] = 'k';
		decode_map['p'] = 'r';
		decode_map['q'] = 'z';
		decode_map['r'] = 't';
		decode_map['s'] = 'n';
		decode_map['t'] = 'w';
		decode_map['u'] = 'j';
		decode_map['v'] = 'p';
		decode_map['w'] = 'f';
		decode_map['x'] = 'm';
		decode_map['y'] = 'a';
		
		decode_map['z'] = 'q';
		decode_map['q'] = 'z';
		
		decode_map[' '] = ' ';
	}
	/*
	int char_to_num(char c) {
		int num = c - 'a';
		return num;
	}
	*/
	std::string decode(std::string i_str) {
		std::string o_str;
		const int size = i_str.size();
		o_str.resize(size);
		
		for(int i = 0; i < size; ++i) {
			o_str[i] = decode_map[i_str[i]];
		}
		return o_str;
	}
};

int main(void) {
	int n_probs = 0;
	std::string str_problem;
	Googlerese decoder;
	
	std::cin >> n_probs;
	std::getline(std::cin, str_problem); // dispose an endline
	for(int i = 0; i < n_probs; ++i) {
		std::cout << "Case #" << (i + 1) << ": ";
		std::getline(std::cin, str_problem);
		std::cout << decoder.decode(str_problem) << std::endl;
	}
	
	return 0;
}

