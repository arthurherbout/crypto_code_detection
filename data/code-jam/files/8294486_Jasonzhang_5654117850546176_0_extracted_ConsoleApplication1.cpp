#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

class Solution
{
public:
	vector<int> result;
	int n;
	bool possible;
	vector<int> f;

	void bt(vector<int>& v)
	{
		if (possible)
			return;
		if (result.size() == n && !near(result.front(), result.back()))
		{
			possible = true;
			f = result;
			return;
		}

		vector<pair<int, int>> vp;
		for (int i = 0; i < 6; ++i)
		{
			vp.push_back({i,v[i]});
		}
		sort(vp.begin(), vp.end(), [](auto& a,auto& b)
		     {
			     return a.second > b.second;
		     });

		auto major = vp[0].second;
		auto rest = n - result.size() - major;
		if (result.empty() == false)
		{
			if (vp[0].first == result.front())
				++major;
			if (vp[0].first == result.back())
				++major;
		}
		if(rest+1<major)
			return;

		for (int i = 0; i < 6; ++i)
		{
			auto idx = vp[i].first;
			if (v[idx] == 0)
				continue;
			if (result.empty() == false && near(idx, result.back()))
				continue;
			result.push_back(idx);
			--v[idx];
			bt(v);
			result.pop_back();
			++v[idx];
		}
	}

	bool near(int a, int b) const
	{
		if (abs(a - b) <= 1)
			return true;

		if (abs(a - b) == 5)
			return true;

		if (a % 2 && b % 2)
			return true;

		return false;
	}

	vector<int> solve(int n, vector<int>& nums)
	{
		result.clear();
		possible = false;
		this->n = n;
		bt(nums);
		return result;
	}
};

int main()
{
	ifstream input_file;
	ofstream output_file;
	output_file.open("output.txt");
	input_file.open("input.txt");
	if (input_file.is_open() == false)
		return -1;

	int t;
	input_file >> t;
	int no = 0;

	string color = "ROYGBV";
	while (t--)
	{
		++no;
		int n;
		vector<int> v(6);

		input_file >> n;
		for (auto&& l : v)
			input_file >> l;

		Solution s;
		s.solve(n, v);
		if (s.possible)
		{
			string re;
			for (auto h : s.f)
				re.push_back(color[h]);

			output_file << "Case #" << no << ": " << re << endl;
			cout << "Case #" << no << ": " << re << endl;
		}
		else
		{
			output_file << "Case #" << no << ": IMPOSSIBLE" << endl;
			cout << "Case #" << no << ": IMPOSSIBLE" << endl;
		}
	}
	return 0;
}
