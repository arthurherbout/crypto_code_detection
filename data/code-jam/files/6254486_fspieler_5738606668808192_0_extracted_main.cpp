#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <vector> 
#include <array>
#include <bitset>
#include <algorithm> 

typedef unsigned long long ll;

using namespace std;

template<int LEN>
class CoinJamGen{
  unsigned _current;
public:
  CoinJamGen():
    _current(0)
  {}
  bool test_coin_jam(const string& candidate_jam, array<unsigned,9>& factors) {
    for(int radix = 2; radix <= 10;++radix)
    {
      ll num = stoll(candidate_jam, 0, radix);
      ll i;
      for(i = 3; i <= 1 + sqrt(num); i+=2)
      {
        if(!(num % i))
        {
          factors[radix-2] = i;
          break;
        }
      }
      if(i >= 1+sqrt(num))
      {
        return false;
      }
    }
    return true;
  }
  string next(){
    array<unsigned,9> factors;
    auto to_str = [this](unsigned x){
      ostringstream oss;
      oss << '1' << std::bitset<LEN-2>(x).to_string() << '1';
      return oss.str();
    };
    for(;!test_coin_jam(to_str(_current), factors);++_current);
    ostringstream ret;
    ret << to_str(_current) ;
    _current++;
    for(auto f : factors){
      ret << ' ' << f;
    }
    return ret.str();
  }
};


string run_test_case(){

    // FEED ME CODE
    CoinJamGen<16> gen16;
    CoinJamGen<32> gen32;
    
    int n, j;
    cin >> n >> j;
    ostringstream ret;
    for(int i = 0; i < j; ++i)
    {
      if(n == 16)
        ret << gen16.next() << endl;
      else
        ret << gen32.next() << endl;
    }
    return ret.str();
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
        cout << "Case #" << test_case << ":\n" << run_test_case();
    }

    return 0;
}
