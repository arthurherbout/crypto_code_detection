#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<iterator>
#include<algorithm>
#include<functional>

class SafetyInNumbers;

class SafetyInNumbers{
public:
	static const double eps = 0.000000001;
	std::vector<double> s_sort;
	std::vector<double> ans;
	
	SafetyInNumbers(){}
	std::vector<double> solve(int N, std::vector<double> s, double sum) {
		s_sort = s;
		ans.resize(N);
		std::sort(s_sort.begin(), s_sort.end(), std::less<double>());
		
		for(int i = 0; i < N; ++i) {
			// std::cerr << "begin: " << i << " " << sum << std::endl;
			ans[i] = bin_search(N, s, sum, i);
		}
		
		return ans;
	}
	double bin_search(int N, std::vector<double> s, double sum, int j) {
		double point_to_get;
		double percent_from = 0, percent_to = 1, percent_now;
		double to_spend_sum;
		
		while(percent_to - percent_from > 0.000000001) {
			percent_now = (percent_from + percent_to)/2;
			point_to_get = s[j] + percent_now * sum;
			to_spend_sum = 0;
			for(int i = 0; i < N; ++i) {
				to_spend_sum += std::max(point_to_get - s[i], 0.0);
				// std::cerr << to_spend_sum << std::endl;
			}
			// std::cerr << percent_now << " " << point_to_get << " " << to_spend_sum << std::endl;
			if(to_spend_sum >= sum) { // not last
				percent_to = percent_now;
			}else{
				percent_from = percent_now;
			}
		}
		return percent_now;
	}
};

int main(void) {
	int n_probs = 0;
	SafetyInNumbers solver;
	
	std::cin >> n_probs;
	int N;
	double sum;
	std::vector<double> s;
	std::vector<double> ans;
	
	std::cout.precision(8);
	
	for(int i = 0; i < n_probs; ++i) {
		std::cin >> N;
		s.resize(N);
		sum = 0;
		// N <= 200, sj <= 100
		for(int j = 0; j < N; ++j) {
			std::cin >> s[j];
			sum += s[j];
		}
		
		std::cout << "Case #" << (i + 1) << ":";
		
		// std::cout << h << w << d;
		// std::cout << std::endl;
		ans = solver.solve(N, s, sum);
		for(int j = 0; j < N; ++j) {
			std::cout << " " << ans[j] * 100;
		}
		std::cout << std::endl;
	}
	
	return 0;
}

