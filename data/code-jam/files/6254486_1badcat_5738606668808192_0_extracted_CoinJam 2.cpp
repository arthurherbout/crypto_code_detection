#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int NB = 9;
int LENGTH;

typedef unsigned long long ull;
typedef vector<ull> ullv;
typedef vector<ullv> vullv;

// Populate all base vectors 2- 10
void populateBaseVectors(vullv & basev);

// Get a "b" base vector where b >=2 and <= 10
void getBaseVector(ullv &bv, int b);

void initBitV(vector<int> &v);

// If this returns 0 then this is a prime number
int getDivisor(ull num);

ull getNumber(vector<int> &v, ullv &bv);

bool checkPrime(vector<int> &v, int& d, int l);

struct ResV
{
	vector<int> v;
	// from 2-10 all divs
	vector<ull> divs;
	ResV(){}
};


// A vector of vector that contains all bases from 2 to 10
vullv allbv;


int main()
{
	int count, len, nums;
	string st;	
	
	cin >> count;
	for (int i = 1; i <= count; ++i)
	{
		cin >> len;
		cin >> nums;
		LENGTH = len;
		populateBaseVectors(allbv);
		vector<int> bitv;
		bitv.reserve(LENGTH);
		initBitV(bitv);
		int MAX = LENGTH - 2;
		vector<ResV> results;
		ull largest = (pow(2,MAX)) -1;
		for (int k = 0; k <= largest; ++k)
		{
			for (int q = 0; q < LENGTH; ++q)
				bitv[q] = 0;

			bitv[0] = 1;
			bitv[LENGTH-1] = 1;
			int num;
			for (int j =0; j < MAX; ++j)
			{
				num = 1;
				num <<= j;
				if (num & k) 
				{
					bitv[j+1] = 1;
				}
			}
			// cout << "Number : " << k << endl;
			// cout << "Bit V: ";
			// for (int z : bitv)
			// 	cout << z << " ";
			// cout << endl;

			ResV res;
			res.v = bitv;

			bool isPrime = false;
			for (int l = 0; l < allbv.size(); ++l)
			{
				int dv = 0;
				isPrime  = checkPrime(bitv, dv, l);
				if (!isPrime) 
					res.divs.push_back(dv);
				else
					break;
			}
			//cout << endl;
			if (isPrime) continue;
			results.push_back(res);
			if (results.size() >= nums) break;
		}

		cout << "Case #" << i << ": " << endl;
		for (ResV rv : results)
		{
			for (int x = rv.v.size() -1; x >=0 ; --x)
				cout << rv.v[x];
			cout << " ";
			for (ull y: rv.divs)
				cout << y << " ";
			cout << endl;
		}
	}
}

void initBitV(vector<int> &v)
{
	for (int i = 0; i < LENGTH; ++i)
		v.push_back(0);
}

// Populate all base vectors 2- 10
void populateBaseVectors(vullv & basev)
{
	basev.clear();
	for (int i = 2 ; i <= 10; ++i)
	{
		ullv bv;
		getBaseVector(bv, i);
		basev.push_back(bv);
	}
	// for (int j = 0; j < basev.size(); ++j)
	// {	
	// 	cout << "Base " << (j+2) << " : ";
	// 	for (ull val : basev[j])
	// 		cout << val << " ";
	// 	cout << endl;
	// }
}

// Get a "b" base vector where b >=2 and <= 10
void getBaseVector(ullv &bv, int b)
{
	bv.clear();
	for (int i = 0; i < LENGTH; ++i)
	{
		bv.push_back(pow(b,i));
	}
}

int getDivisor(ull tot) {
    if (tot >= 1 && tot <= 3) {
        return 0;
    } else if (tot % 2 == 0) { return 2;
    } else if (tot % 3 == 0) { return 3;
    } else {
        for (int a = 5; a * a <= tot; a += 6) {
            if (tot % a == 0) return a;
            if (tot % (a + 2) == 0) return a+2;
            }
        }
        return 0;
}


bool checkPrime(vector<int> &v, int& d, int l)
{
	ull tot = 0;
	for (int m = 0; m < v.size(); ++ m)
	{
		tot += v[m] * allbv[l][m];
	}
	//cout << " " << tot;

	d = getDivisor(tot);
	// getDivisor returns 0 if this is  prime number
	if (d == 0) return true;
	return false;
}
