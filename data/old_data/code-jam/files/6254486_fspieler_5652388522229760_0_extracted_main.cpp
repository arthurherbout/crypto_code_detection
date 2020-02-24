#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector> 
#include <array>
#include <algorithm> 


using namespace std;

template<typename T>
string tos(T x)
{
  ostringstream oss;
  oss << x;
  return oss.str();
}

bool all_digits(unsigned number, array<bool,10>& digits)
{
  while(number)
  {
    unsigned new_digit = number % 10;
    number /= 10;
    digits[new_digit] = true;
  }
  for(auto digit : digits)
  {
    if(!digit)
    {
      return false;
    }
  }
  return true;
}

string run_test_case(){

    // FEED ME CODE
    unsigned initial;
    cin >> initial;
    if(!initial){
      return "INSOMNIA";
    }
    array<bool,10> digits = {{0}};
    for(unsigned factor = 1;;++factor)
    {
      auto number = initial * factor;
      if(all_digits(number,digits))
      {
        return tos(number);
      }
    }
    
    return "10";
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
