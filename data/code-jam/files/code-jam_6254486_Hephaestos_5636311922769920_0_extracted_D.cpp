#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
using ull = unsigned long long;


string SolveD(int K, int C) {
	vector<ull> result(K);
	for (ull i = 0;i < result.size();++i) {
		result[i] = i;
	}
	ull sz = 1;
	for (int i = 1;i < C;++i) {
		sz *= ull(K);
	}
	for_each(result.begin(), result.end(), [sz](auto& r) {
		r *= sz;
	});
	stringstream ss;
	for_each(result.begin(), result.end(), [&ss](const auto& r) {
		ss<<' '<<r+1;
	});
	return ss.str();
}