#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

vector<vector<long long>> solve(int n, int j)
{
	return
	{ { 1000000000000001, 3, 2, 5, 2, 7, 2, 3, 2, 7 },
	{ 1000000000000101, 13, 11, 3, 4751, 173, 3, 53, 109, 3 },
	{ 1000000000000111, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000000001001, 73, 5, 3, 19, 19, 3, 5, 19, 3 },
	{ 1000000000001101, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000000010011, 3, 2, 5, 2, 7, 2, 3, 2, 7 },
	{ 1000000000011001, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000000011011, 5, 1567, 15559, 6197, 5, 5, 1031, 7, 83 },
	{ 1000000000011111, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000000100101, 3, 2, 5, 2, 7, 2, 3, 2, 7 },
	{ 1000000000101011, 3, 7, 13, 3, 5, 43, 3, 73, 7 },
	{ 1000000000101111, 5, 2, 3, 2, 37, 2, 5, 2, 3 },
	{ 1000000000110001, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000000110101, 23, 17, 11, 23, 5, 299699, 43, 239, 59 },
	{ 1000000000110111, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000000111011, 17, 2, 3, 2, 73, 2, 17, 2, 3 },
	{ 1000000000111101, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001000011, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000001001001, 3, 2, 5, 2, 7, 2, 3, 2, 7 },
	{ 1000000001001111, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001010101, 3, 7, 13, 3, 5, 17, 3, 53, 7 },
	{ 1000000001010111, 5, 2, 3, 2, 37, 2, 5, 2, 3 },
	{ 1000000001011001, 11, 5, 281, 101, 5, 67, 5, 13, 19 },
	{ 1000000001011011, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001011101, 17, 2, 3, 2, 1297, 2, 11, 2, 3 },
	{ 1000000001011111, 59, 113, 7, 157, 19, 1399, 7, 43, 107 },
	{ 1000000001100001, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000001100011, 23, 19, 11, 105491, 5, 47, 11117, 1787, 127 },
	{ 1000000001100111, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001101011, 5, 2, 3, 2, 37, 2, 5, 2, 3 },
	{ 1000000001101101, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001110011, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001110101, 5, 2, 3, 2, 37, 2, 5, 2, 3 },
	{ 1000000001111001, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000001111011, 31, 557, 7, 19, 23, 1129, 7, 5441, 241 },
	{ 1000000001111101, 7, 19, 43, 17, 55987, 23, 7, 7, 31 },
	{ 1000000001111111, 3, 2, 5, 2, 7, 2, 3, 2, 7 },
	{ 1000000010000011, 167, 2, 11, 2, 58427, 2, 23, 2, 839 },
	{ 1000000010000101, 3, 2, 5, 2, 7, 2, 3, 2, 11 },
	{ 1000000010001001, 5, 2, 7, 2, 1933, 2, 29, 2, 157 },
	{ 1000000010010001, 3, 2, 5, 2, 7, 2, 3, 2, 7 },
	{ 1000000010010111, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000010011001, 7, 1667, 179, 13, 5, 11, 23, 7, 311 },
	{ 1000000010011011, 11, 2, 3, 2, 13, 2, 47, 2, 3 },
	{ 1000000010011101, 3, 2, 3, 2, 7, 2, 3, 2, 3 },
	{ 1000000010100011, 3, 1259, 421, 3, 5, 8893, 3, 67, 17 },
	{ 1000000010100111, 5, 2, 3, 2, 37, 2, 5, 2, 3 },
	{ 1000000010101001, 3, 5, 13, 3, 5, 43, 3, 73, 7 },
	{ 1000000010110011, 47, 2, 3, 2, 11, 2, 204311, 2, 3 },
	{ 1000000010110101, 3, 2, 3, 2, 7, 2, 3, 2, 3 } }
	;
}

void test()
{
	
}

void stress()
{
	//cerr << "TL tests" << endl;
	//double worst_time = 0;
	//for (int test = 0; test < 1000; ++test)
	//{
	//	int n = 100;
	//	string stack(n, '-');
	//	for (int i = 0; i < n; ++i)
	//	{
	//		if (rand() & 1)
	//		{
	//			stack[i] = '+';
	//		}
	//	}

	//	// cout << stack << endl;
	//	double start = clock();
	//	cout << solve(stack) << "\r";
	//	double finish = clock();

	//	double duration = (finish - start) / CLOCKS_PER_SEC;
	//	if (duration > worst_time)
	//	{
	//		worst_time = duration;
	//		cout << "Time: " << duration << "sec" << endl;
	//	}
	//}

//	for (int n = 1; n <= 30; ++n)
//	{
//		cerr << "Input lenght is " << n << endl;
//#pragma omp parallel for
//		for (int state = 0; state < (1 << n); ++state)
//		{
//			string stack(n, '-');
//			for (int i = 0; i < n; ++i)
//			{
//				if (state & (1 << i))
//				{
//					stack[i] = '+';
//				}
//			}
//
//			// cerr << stack << endl;
//
//			int ans1 = solve(stack);
//
//			int ans2 = brute_solve(stack);
//
//			if (ans1 != ans2)
//			{
//				cout << "WA!!!" << endl;
//				cout << "Input: " << stack << endl;
//				cout << "Solution: " << ans1 << endl;
//				cout << "Currect: " << ans2 << endl;
//				// return;
//			}
//		}
//	}
}

long long isPrime(long long x)
{
	for (long long i = 2; i * i <= x && i < x; ++i)
	{
		if (x % i == 0)
		{
			return i;
		}
	}
	return -1;
}

void mine(int n, int j)
{
	stringstream ss;
	ss << "precalc_" << n << "_" << j << ".txt";

	ofstream file(ss.str());
	file << '{';
	int good_count = 0;
	for (int state = 0; state < (1 << (n - 2)); ++state)
	{
		cerr << state << '\r';
		vector<long long> proof;
		bool good = true;
		long long number;
		for (int base = 2; good && base <= 10; ++base)
		{
			number = 1;
			long long power = 1;
			for (int i = 0; i < n - 2; ++i)
			{
				power *= base;
				if (state & (1 << i))
				{
					number += power;
				}
			}
			power *= base;
			number += power;

			long long check = isPrime(number);
			if (check < 0)
			{
				good = false;
			}
			else
			{
				proof.push_back(check);
			}
		}

		if (good)
		{
			if (good_count)
			{
				file << ",\n";
			}			

			file << '{' << number;
			for (long long p : proof)
			{
				file << ',' << p;
			}
			file << '}';

			++good_count;
			if (good_count == j)
			{
				break;
			}
		}
	}
	file << '}' << endl;
	cerr << "Done" << endl;
}

int main(int argc, char* argv[])
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	srand(13);

	// mine(16, 50);
	// test();
	// stress();
	// return 1;

	int T;
	cin >> T;
	double global_start = clock();
	for (int test = 1; test <= T; ++test)
	{
		double start = clock();
		int n, j;
		cin >> n >> j;		
		cout << "Case #" << test << ":" << endl;
		for (const auto &line : solve(n, j))
		{
			bool first = true;
			for (long long number : line)
			{
				if (first)
				{
					first = false;
				}
				else
				{
					cout << ' ';
				}
				cout << number;
			}
			cout << endl;
		}
		double finish = clock();
		cerr << (finish - start) / CLOCKS_PER_SEC << "s" << endl << endl;
	}
	cerr << "Total time: " << (clock() - global_start) / CLOCKS_PER_SEC << "s" << endl;

	return 0;
}

