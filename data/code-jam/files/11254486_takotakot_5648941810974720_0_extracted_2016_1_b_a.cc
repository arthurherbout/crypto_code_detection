#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<iterator>

struct solver_2016_1_b_a {
  // use 0-origin


  std::map<char, int> char_occur_count;
  std::map<int, int> digit_occur_count;
  // std::map<char, int> dict;
  std::map<int, std::map<char, int> > digit_dict;

  solver_2016_1_b_a() {
    // ZERO
    digit_dict[0]['Z'] = 1;
    digit_dict[0]['E'] = 1;
    digit_dict[0]['R'] = 1;
    digit_dict[0]['O'] = 1;
    // ONE
    digit_dict[1]['O'] = 1;
    digit_dict[1]['N'] = 1;
    digit_dict[1]['E'] = 1;
    // TWO
    digit_dict[2]['T'] = 1;
    digit_dict[2]['W'] = 1;
    digit_dict[2]['O'] = 1;
    // THREE
    digit_dict[3]['T'] = 1;
    digit_dict[3]['H'] = 1;
    digit_dict[3]['R'] = 1;
    digit_dict[3]['E'] = 2;
    // FOUR
    digit_dict[4]['F'] = 1;
    digit_dict[4]['O'] = 1;
    digit_dict[4]['U'] = 1;
    digit_dict[4]['R'] = 1;
    // FIVE
    digit_dict[5]['F'] = 1;
    digit_dict[5]['I'] = 1;
    digit_dict[5]['V'] = 1;
    digit_dict[5]['E'] = 1;
    // SIX
    digit_dict[6]['S'] = 1;
    digit_dict[6]['I'] = 1;
    digit_dict[6]['X'] = 1;
    // SEVEN
    digit_dict[7]['S'] = 1;
    digit_dict[7]['E'] = 2;
    digit_dict[7]['V'] = 1;
    digit_dict[7]['N'] = 1;
    // EIGHT
    digit_dict[8]['E'] = 1;
    digit_dict[8]['I'] = 1;
    digit_dict[8]['G'] = 1;
    digit_dict[8]['H'] = 1;
    digit_dict[8]['T'] = 1;
    // NINE
    digit_dict[9]['N'] = 1;
    digit_dict[9]['I'] = 1;
    digit_dict[9]['N'] = 1;
    digit_dict[9]['E'] = 1;
    // TEN
    digit_dict[10]['T'] = 1;
    digit_dict[10]['E'] = 1;
    digit_dict[10]['N'] = 1;

    return;
  }

  std::string run(std::string S) {
    std::stringstream ss;

    char_occur_count.clear();
    digit_occur_count.clear();

    for(int i = 0; i < S.size(); ++i) {
      ++char_occur_count[S[i]];
    }

    /*
    for(std::map<char, int>::iterator it = char_occur_count.begin(); it != char_occur_count.end(); ++it) {
      std::cerr << it->first << "\t" << it->second << std::endl;
    }
    */
    std::map<char, int> coc = char_occur_count;

    int digits[] = {0, 2, 6, 8, 3, 4, 5, 7, 1, 9, 10};
    char characters[] = {'Z', 'W', 'X', 'G', 'H', 'R', 'F', 'V', 'O', 'I', 'T'};
    int digit;
    char character;
    for(int i = 0; i < 10; ++i) {
      digit = digits[i];
      character = characters[i];
      digit_occur_count[digit] = coc[character];
      // std::cerr << digit << "\t" << character << "\t" << digit_occur_count[digit] << std::endl;
      for(std::map<char, int>::iterator it = digit_dict[digit].begin(); it != digit_dict[digit].end(); ++it) {
        // std::cerr << it->first << "\t" << digit_occur_count[digit] * it->second << std::endl;
        coc[it->first] -= digit_occur_count[digit] * it->second;
      }
      // std::cerr << std::endl;
    }

    for(std::map<int, int>::iterator it = digit_occur_count.begin(); it != digit_occur_count.end(); ++it) {
      // std::cerr << it->first << "\t" << it->second << std::endl;
      for(int i = 0; i < it->second; ++i) {
        ss << it->first;
      }
    }

    return ss.str();
  }

};

int main(void) {
  solver_2016_1_b_a solver;

  int T;
  std::string S;

  std::cin >> T;
  for (int i = 0; i < T; ++i) {
    std::cin >> S;
    std::cerr << S << std::endl;
    // std::cerr << "Case #" << (i + 1) << ": " << solver.run(S) << std::endl;
    std::cout << "Case #" << (i + 1) << ": " << solver.run(S) << std::endl;
  }

  return 0;
}
