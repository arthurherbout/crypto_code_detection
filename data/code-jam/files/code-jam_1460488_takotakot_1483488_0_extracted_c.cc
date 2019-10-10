#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<iterator>
#include<algorithm>
#include<functional>

class RecycledNumbers;

class RecycledNumbers{
public:
	int n_digits;
	int max_in_ten;
	std::map<int, bool> counted_map;
	// std::map<int, map<int, bool> counted_pair_map;
	RecycledNumbers(){}
	int recycle_one(int n) {
		int m = (n % 10) * max_in_ten + n/10;
		return m;
	}
	void setup(int a) {
		int n_digits_tmp = 1;
		int max_in_ten_tmp = 1;
		while(a >= 10) {
			a /= 10;
			++n_digits_tmp;
			max_in_ten_tmp *= 10;
		}
		n_digits = n_digits_tmp;
		max_in_ten = max_in_ten_tmp;
		// std::cout << n_digits << " " << max_in_ten << std::endl;
		return;
	}
	int solve(int a, int b) {
		int ans = 0;
		setup(a);
		
		for(int n = a; n <= b; ++n) {
			int m = n;
			counted_map.clear();
			for(int i = 1; i < n_digits; ++i) {
				// std::cout << m << " to ";
				m = recycle_one(m);
				// std::cout << m << std::endl;
				if(n < m) {
					if(a <= m && m <= b) {
						if(!counted_map[m]) {
							ans++;
							counted_map[m] = true;
						}
					}
				}
			}
		}
		return ans;
	}
};

int main(void) {
	int n_probs = 0;
	RecycledNumbers solver;
	
	std::cin >> n_probs;
	int a, b;
	// std::vector<int> n;
	
	for(int i = 0; i < n_probs; ++i) {
		int ans;
		std::cin >> a >> b;
		
		std::cout << "Case #" << (i + 1) << ": ";
		std::cout << solver.solve(a, b) << std::endl;
	}
	
	return 0;
}

