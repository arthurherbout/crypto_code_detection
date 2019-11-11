#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::max;

const int BLUE = 0;
const int ORANGE = 1;

void Input(vector<vector<char>>* combines, vector<vector<char>>* opposed, vector<char>* series) { 
  combines->clear();
  opposed->clear();
  series->clear();

  int number;
  cin >> number;
  for (int i = 0; i < number; ++i) { 
    string str;
    cin >> str;
    vector<char> fromstr(str.begin(),str.end());
    combines->push_back(fromstr);
  }

  cin >> number;
  for (int i = 0; i < number; ++i) { 
    string str;
    cin >> str;
    vector<char> fromstr(str.begin(),str.end());
    opposed->push_back(fromstr);
  }

  cin >> number;
  string str;
  cin >> str;
  vector<char> fromstr(str.begin(),str.end());
  *series = fromstr;
}
    

void FindOpposed(const vector<vector<char>>& opposed, vector<char>& result) {
  if (result.size() >= 2) {
    for (int i = 0; i < opposed.size(); ++i) { 
      for (int second_count = 0; second_count < result.size() - 1; ++second_count) {
        if (  ( result.back() == opposed[i][1] && result[second_count] == opposed[i][0]) ||
            ( result.back() == opposed[i][0] && result[second_count] == opposed[i][1]) )  {
          result.clear();
          return;
        }
      }
    }
  }
}

vector<char> Answer(const vector<vector<char>>& combines, const vector<vector<char>>& opposed, const vector<char>& series) {
  vector<char> result;
  for (int char_count = 0; char_count < series.size(); ++char_count) {
    result.push_back(series[char_count]);

    //combines
    if (result.size() >= 2) {
      for (int i = 0; i < combines.size(); ++i) {
        if (  ( result.back() == combines[i][1] && result[result.size()-2] == combines[i][0]) ||
              ( result.back() == combines[i][0] && result[result.size()-2] == combines[i][1]) )  {
          result.pop_back();
          result.pop_back();
          result.push_back(combines[i][2]);
          break;
        }
      }
    }

    //opposed
    FindOpposed(opposed, result);  

  }
  return result;
}

void Output(int iter, const vector<char>& answers) {
   cout << "Case #" << iter + 1 << ": " << "[";
  for (int i = 0; i < answers.size(); ++i) {
    cout << answers[i];
    if (i+1 < answers.size()) { 
      cout << ", ";
    }
  }
  cout << "]" << endl;
}

int main() {
  int T;
  cin >> T;
  for (int i = 0; i < T; ++i) {
    vector<vector<char>> combines;
    vector<vector<char>> opposed;
    vector<char> series;
    
    Input(&combines, &opposed, &series);
    Output(i, Answer(combines, opposed, series));

  }

  return 0;
}