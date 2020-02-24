#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector> 
#include <algorithm> 


using namespace std;

template<typename T>
string tos(T in)
{
  ostringstream oss;
  oss << in;
  return oss.str();
}

unsigned run_test_case(){

    string input;
    cin >> input;
    bool head = (input[0] == '+');
    if(input.size() <= 1)
    {
      return !head;
    }
    string tail = input.substr(1);
    bool last = head;
    unsigned count = 0;
    for(auto c : tail)
    {
      bool facing_up = (c == '+');
      if(facing_up != last)
      {
        count++;
      }
      last = facing_up;
    }
    
    return count + (!(head ^ (count%2)));
}

int main(int argc, char* argv[]){

    int num_test_cases;
    int start;
    cerr << argc << endl;
    if(argc == 3){
        num_test_cases = atoi(argv[1]);
        start = atoi(argv[2]);
    } else {
        cin >> num_test_cases;
        start = 1;
    }

    for (int test_case = start; test_case < start+num_test_cases; test_case++){
        cout << "Case #" << test_case << ": " << run_test_case() << endl;
    }

    return 0;
}
