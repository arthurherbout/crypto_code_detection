#include <iostream>
#include <fstream>
#include <set>
#include <list>
#include <vector>
#include <map>

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

typedef pair<long long int,long long int> IIPair;

//additional infos
int num;//numbver of possibilities
int n;//num of trees
long long int A;
long long int B;
long long int C;
long long int D;
long long int x0;
long long int y0;
long long int M;
vector<IIPair> trees;

void gen(int _n){
	long long int X = x0;
	long long int Y = y0;
	trees.clear();
	trees.resize(_n);
	trees[0].first = X;
	trees[0].second = Y;
	for(int i = 1; i < _n; ++i){
		X = (A * X + B)%M;
		Y = (C * Y + D)%M;
		trees[i].first = X;
		trees[i].second = Y;
	}

}
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
		//read input
		is >> n;
		is >> A;
		is >> B;
		is >> C;
		is >> D;
		is >> x0;
		is >> y0;
		is >> M;

		//do the stuff
		gen(n);
		num = 0;
		for(int i = 0; i < n-2; ++i){
			for(int j = i+1; j < n-1; ++j){
				for(int k = j+1; k < n; ++k){
					if((trees[i].first + trees[j].first + trees[k].first)%3 == 0 && (trees[i].second + trees[j].second + trees[k].second)%3 == 0){
						++num;
					}
				}
			}
		}

		//write result
		os << "Case #" << i+1 << ": " << num << endl;
	}

	cout << "Job done. Solved cases: " << nCase << endl << "Byebye!" << endl;
	return 0;
}