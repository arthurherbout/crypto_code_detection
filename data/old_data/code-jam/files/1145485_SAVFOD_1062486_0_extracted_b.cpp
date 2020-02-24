#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

using std::vector;
using std::string;
using std::set;
using std::multiset;
using std::cin;
using std::cout;
using std::endl;
using std::sort;
using std::lower_bound;

vector<int> MakeMask(const string& str, char c) {
  vector<int> answer;
  for(int i = 0; i < str.size(); ++i) {
    int is_match = (str[i] == c) ? 1 : 0;
    answer.push_back(is_match);
  }
  return answer;
}

int MakeInt(vector<int> mask){
  int ans = 0;
  
  for (int j = 0; j < mask.size(); ++j) {
    ans += (mask[j] << j);
  }

  return ans;
}

void CreateWordsIndexes(const vector<string> words, vector<vector<int>>* words_indexes, const string& list) {
  words_indexes->clear();
  vector<int> default_indexes(26, -1);
  for (int i = 0; i < words.size(); ++i) {
    words_indexes->push_back(default_indexes);
  }
  
  for (int c_index = 0; c_index < list.size(); ++c_index) {
    char c = list[c_index];

    vector<int> words_masks;
    for (int i = 0; i < words.size(); ++i) {
      words_masks.push_back(MakeInt(MakeMask(words[i], c)));
    }
    sort(words_masks.begin(), words_masks.end());

    for (int i = 0; i < words.size(); ++i) {
      int index = lower_bound(words_masks.begin(), words_masks.end(), MakeInt(MakeMask(words[i], c))) - words_masks.begin();
      (words_indexes->at(i)).at(c-'a') = index;
    }
  }
}

int Common(vector<int> first, vector<int> second) {
  int i = 0;
  for (; i < first.size(); ++i) {
    if (first[i] != second[i]) {
      return i;
    }
  }
  return i;
}



void Solve(const vector<string>& words, const vector<string>& lists, vector<int>* answer_indexes) {
  answer_indexes->clear();
  vector<vector<int>> words_indexes;
  
  /*vector<vector<int>> words_indexes_copy = words_indexes;
  sort(words_indexes_copy.begin(), words_indexes_copy.end());*/

  for (int list_index = 0; list_index < lists.size(); ++list_index) {
    CreateWordsIndexes(words, &words_indexes, lists[list_index]);
    
    vector<vector<int>> words_indexes_copy = words_indexes;
    sort(words_indexes_copy.begin(), words_indexes_copy.end());
    
    int max_same = 0;
    int max_same_index = 0;

    for (int i = 0; i + 1 < words_indexes_copy.size(); ++i) {  
      if (Common(words_indexes_copy[i], words_indexes_copy[i+1]) > max_same) {
        max_same = Common(words_indexes_copy[i], words_indexes_copy[i+1]);
        max_same_index = i;
      }
    }

    for (int i = words_indexes.size() - 1; i >= 0; i--) {
      if (Common(words_indexes[i],words_indexes_copy[max_same_index]) == max_same) {
        answer_indexes->push_back(i);
        break;
      }
    }

    //answer_indexes->push_back()
  }
}

void Input(vector<string>* first, vector<string>* second) {
  int first_size, second_size;
  cin >> first_size >> second_size;

  for( int i = 0; i < first_size; ++i) {
    string str;
    cin >> str;
    first->push_back(str);
  }

  for( int i = 0; i < second_size; ++i) {
    string str;
    cin >> str;
    second->push_back(str);
  }

}

void Output(const vector<string>& words, const vector<int>& indexes, int i) {
  cout << "Case #" << i+1 << ":";
  for (int k = 0; k < indexes.size(); ++k) {
    cout << " " << words[indexes[k]];
  }
  cout << endl;
}

int main() {
  int T;
  
  cin >> T;

  for( int i = 0; i < T; ++i ) {
    vector<string> words;
    vector<string> lists;
    Input(&words, &lists);

    vector<int> answer_indexes;
    Solve(words, lists, &answer_indexes);
    Output(words, answer_indexes, i);


  }

  return 0;
}