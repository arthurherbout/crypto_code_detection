#include <iostream>
#include <fstream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <algorithm>


using namespace std;

typedef list<int> IList;
typedef list<double> DList;
typedef list<string> SList;
typedef list<char> CList;

typedef vector<int> IVector;
typedef vector<double> DVector;
typedef vector<string> SVector;
typedef vector<char> CVector;

typedef set<int> ISet;
typedef set<double> DSet;
typedef set<string> SSet;
typedef set<char> CSet;

typedef pair<int,int> IIPair;


int L;
int P;
int K;
long long int num;

vector<IIPair> freq;//first is frequency and second is the letter position
IVector pressneed;

int main(int argc, char* argv[])
{
	//check if input file exist
	if(argc != 2){
		cout << "Wrong input parameters: " << argc << endl << "It shall be 2.";
		return -1;
	}
	//input file
	ifstream is;
	is.open(argv[1]);
	if(is.fail()){
		cout << "Input file open failed!" << endl;
		return -2;
	}
	//output file
	string osf = argv[1];
	osf.append(".out");
	ofstream os;
	os.open(osf.c_str());
	if(os.fail()){
		cout << "Output file open failed!" << endl;
		return -3;
	}
	int nCase;//number of cases
	is >> nCase;
	for(int i = 0; i < nCase; ++i){
		num = 0;
		is >> P;
		is >> K;
		is >> L;
		pressneed.resize(L);
		freq.resize(L);
		for(int j = 0; j < L; ++j){
			is >> freq[j].first;
			freq[j].second = j;
		}
		sort(freq.begin(),freq.end());
		int pn = 0;
		int step = 0;
		for(int j = freq.size(); j > 0;--j, ++step){
			if(step%K==0){
				++pn;
			}
			num += (freq[j-1].first*pn);
		}
		
		os << "Case #" << i+1 << ": " << num << endl;
	}

	cout << "Job done. Solved cases: " << nCase << endl << "Byebye!" << endl;
	return 0;
}