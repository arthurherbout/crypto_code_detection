#include<string>
#include <set>
#include<iostream>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#if   defined   (_INTEGRAL_MAX_BITS)   &&   _INTEGRAL_MAX_BITS   >=   64  
typedef   signed   __int64    int64;          
typedef   unsigned   __int64   uint64;  
#else  
#error   __int64   type   not   supported  
#endif   

int IsPower(int64 n,int k){
	while (n%k==0){
		n=n/k;
		if (n==1) return 1;
	}
	return 0;
}

int IsHappy(int64 n,int base){
	set<int64> myset;
	while (1){
		if (IsPower(n,base)) return 1;
		else{
			if (myset.find(n) == myset.end()){
				myset.insert(n);
				int64 sum=0;
				while (n>0){
					sum+= ((n%base)*(n%base));
					n=n/base;
				}
				n =sum;
			}
			else
				return 0;
		}
		
	}
	return 1;
}


void main(){
	filebuf ifb;
	ifb.open ("A-small-attempt0.in",ios::in);
	istream min(&ifb);
	//istream min(cin);
	
	filebuf ofb;
	ofb.open ("A-small-attempt0.out.txt",ios::out);
	ostream mout(&ofb);
	//ostream mout(cout);

	int t;
	min >> t;
	
	string line;
	getline(min,line);
	
	int i, j;
	for(i = 0; i < t; i++){
		string line;
		getline(min,line);
		istringstream instream;
		instream.clear();
		instream.str(line);
		vector<int> bases;
		j = 0;
		while(instream >> j){
			bases.push_back(j);
		}
		vector<int> ::iterator iter;
		for (int64 test=2;;test++){
			for (iter=bases.begin();iter!=bases.end();iter++){
				if (!IsHappy(test,(*iter)))break;
			}
			
			if (iter ==bases.end()){
				printf("Case #%d: %I64d\n",i+1,test);
				break;
			}
		}
		
		// 		cout << "i = " << i << " line = " << line << endl;
		// 		cout << "bases size = " << bases.size() << endl;
	}//for each test case
	
	
	
}