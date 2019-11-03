#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
using namespace std;

//#define DEBUG
#define MAX_NUM  200

class BigInteger{
public:
  int digits[MAX_NUM];
  BigInteger(int b){
    memset(digits,0,sizeof(int)*MAX_NUM);
    digits[0] = b;
    if(digits[0] > 9)
      format();
  }

  BigInteger(const BigInteger& b){
    memset(digits,0,sizeof(int)*MAX_NUM);
    copy(b.digits,b.digits+MAX_NUM,digits);
  }

  friend const BigInteger operator+(const BigInteger& left, const BigInteger& right){
    BigInteger bi(0);
    for(int i = 0; i < MAX_NUM; i++){
      bi.digits[i] =  left.digits[i] + right.digits[i];
    }
    bi.format();
    return bi;
  }

  friend const BigInteger operator*(const BigInteger& left, const int& right){
#ifdef DEBUG
		cout << "##left=" << left << endl;
		cout << "##right=" << right << endl;
#endif
    BigInteger bi(0);
    for(int i = 0; i < MAX_NUM; i++){
      bi.digits[i] =  left.digits[i] * right;
	  /*
#ifdef DEBUG
		cout << "left.digits[" << i << "]=" << left.digits[i] << endl;
		cout << "bi.digits[" << i << "]=" << bi.digits[i] << endl;
#endif
		*/
    }
    bi.format();
#ifdef DEBUG
		cout << "##bi=" << bi << endl;
		cout << endl;
#endif
    return bi;
  }
/*
   friend const BigInteger operator*(const BigInteger& left, const BigInteger& right){
#ifdef DEBUG
		cout << "##left=" << left << endl;
		cout << "##right=" << right << endl;
#endif
    BigInteger bi(0);
    for(int i = 0; i < MAX_NUM/2-1; i++){
		for(int j = 0; j < MAX_NUM/2-1; j++){
			bi.digits[i+j] +=  left.digits[i] * right.digits[j];
		}
    }
#ifdef DEBUG
		cout << "##bi=" << left << endl;
#endif
    bi.format();
    return bi;
  }
*/
  friend ostream& operator << (ostream &out, BigInteger const& bi){
    bool first_nonzero = false;
    for(int i = MAX_NUM-1; i >= 0; i--){
      if(i == 0)
	first_nonzero = true;
      if(first_nonzero == false){
	if(bi.digits[i] > 0){
	  first_nonzero = true;
	  out << bi.digits[i];
	}
      }
      else{
	out << bi.digits[i];
      }
    }//for
    return out;
  }

  void format(){
    for(int i = 0; i < MAX_NUM; i++){
      if(digits[i] >= 10){
	digits[i+1] += digits[i] / 10;
	digits[i] = digits[i] % 10;
      }//if
    }//for
  }//format
};

int main(){
	filebuf ifb;
	ifb.open ("A-small-attempt2.in",ios::in);
	istream min(&ifb);
	//istream min(min);
	
	filebuf ofb;
	ofb.open ("A-small-attempt2.out.txt",ios::out);
	ostream mout(&ofb);
	//ostream mout(mout);
	
	int t;
	min >> t;
	int i, j;
	for(i = 0; i < t; i++){
		vector<char> number;
		set<char> symbols;
		string line;
		min >> line;
		for(j = 0; j < line.size(); j++){
			number.push_back(line[j]);
			symbols.insert(line[j]);
		}//for each digits in the number
		int base = symbols.size();
		if(base == 1)
			base = 2;
#ifdef DEBUG
		cout << "base=" << base << endl;
#endif
		map<char,int> symbol_weight;
		int bi = 1;
		symbol_weight.insert(make_pair(number[0], bi));
#ifdef DEBUG
		cout << "char = " << number[0] << " weight = " << bi << endl;
#endif
		bi++;
		bool zero = true;
		for(j = 1; j < number.size(); j++){
			if(symbol_weight.count(number[j]) == 0){
				if(zero == true){
					zero = false;
					int bi0 = 0;
					symbol_weight.insert(make_pair(number[j],bi0));	
#ifdef DEBUG
		cout << "char = " << number[j] << " weight = 0" << endl;
#endif
				}
				else{
					symbol_weight.insert(make_pair(number[j],bi));	
#ifdef DEBUG
		cout << "char = " << number[j] << " weight = " << bi << endl;
#endif
					bi++;
				}
			}
		}

		BigInteger weight(1);
		BigInteger res(0);
		for(j = number.size()-1; j >= 0; j--){
#ifdef DEBUG
			cout << "\tenter weight = " << weight << endl;
#endif
			int bt = symbol_weight.find(number[j])->second;
			BigInteger btt = weight * bt;
			res = res + btt;
#ifdef DEBUG
		cout << "num = " << number[j] << endl;
		cout << "\tbt = " << bt << " weight=" << weight << " btt=" << btt << endl;
		cout << "\tweight = " << weight << endl;
		cout << "\tres = " << res << endl;
		cout << endl;
		cout << "weight:" << endl;
#endif

			weight = weight * base;
#ifdef DEBUG
			cout << "\tleave weight = " << weight << endl;
			cout << "---------------------------------------" << endl;
#endif
		}//for j
		mout << "Case #" << i+1 << ": " << res << endl;
	}//each test case
	return 0;
}
