#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

#define ll long long

const int MAX_N = 20;

void do_case(){
  int N;
  cin >> N;
  
  vector<int> enter,leave;
  
  int in_house[2300];
  bool know_something[2300] = {0};
  fill_n(in_house,2300,0);
  
  char c;
  int p;
  bool possible = true;
  set<int> people;
  for(int i=1;i<=N;i++){
    cin >> c >> p;
    people.insert(p);
    if(p == 0){
      if(c == 'E') enter.push_back(i);
      if(c == 'L') leave.push_back(i);
      continue;
    }
    if(c == 'E'){
      if(in_house[p] > 0){
	if(leave.empty() || leave[leave.size()-1] < in_house[p]){
	  possible = false;
	} else {
	  
	  for(int j=0;j<leave.size();j++){
	    if(leave[j] > in_house[p]){
	      leave.erase(leave.begin()+j);
	      break;
	    }
	  }
	}
      }
      in_house[p] = i;
    } else if(c == 'L'){
      if(in_house[p] == 0){
	if(enter.empty()){
	  if(know_something[p])
	    possible = false;
	} else {
	  enter.erase(enter.begin());
	}
      }
      in_house[p] = 0;
    } else {
      assert(false);
    }
    know_something[p] = true;
  }
  
  if(!possible){
    cout << "CRIME TIME" << endl;
  } else {
    // How many people?
    int ctr = 0;
    for(int i=0;i<2030;i++)
      if(in_house[i] > 0)
	ctr++;
    //cout << ctr << " " << enter.size() << " " << leave.size() << endl;
    ctr = ctr + enter.size();// - leave.size();
    if(ctr < 0) ctr = 0;
    cout << ctr << endl;
  }
}

int main(){
  int T,C=1;
  cin >> T;
  while(T--){
    cout << "Case #" << C++ << ": ";
    do_case();
  }
  
  return 0;
}
