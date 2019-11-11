#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<iterator>
#include<algorithm>
#include<functional>

typedef std::vector<std::set<int> > ans_type;
typedef std::map<long long, std::set<int> > sum_type;
class TideGoesInOut;

class TideGoesInOut{
public:
	static const double eps = 0.000000001;
	
	TideGoesInOut(){}
	ans_type solve(int N, std::vector<int> s) {
		std::set<int> empty;
		ans_type ans(2, std::set<int>());
		std::map<long long, std::set<int> > available_sum_old, available_sum_new;
		available_sum_new[0] = empty;
		bool flag = false;
		
		for(int i = 0; i < N; ++i) {
			sum_type::iterator it_temp;
			int adding = s[i];
			available_sum_old = available_sum_new;
			for(sum_type::iterator it = available_sum_old.begin(); it != available_sum_old.end(); ++it) {
				it_temp = available_sum_old.find(it->first + adding);
				if(it_temp == available_sum_old.end()) { // add new
					available_sum_new[it->first + adding] = it->second;
					available_sum_new[it->first + adding].insert(adding);
				}else{ // find!
					ans[0] = it_temp->second;
					ans[1] = it->second;
					ans[1].insert(adding);
					flag = true;
					break;
				}
			}
			if(flag) {
				break;
			}
		}
		return ans;
	}
};

int main(void) {
	int n_probs = 0;
	TideGoesInOut solver;
	
	std::cin >> n_probs;
	int N;
	double sum;
	std::vector<int> s;
	// std::vector<double> ans;
	ans_type ans(2, std::set<int>());
	
	std::cout.precision(8);
	
	for(int i = 0; i < n_probs; ++i) {
		std::cin >> N;
		s.resize(N);
		sum = 0;
		// N <= 200, sj <= 100
		for(int j = 0; j < N; ++j) {
			std::cin >> s[j];
		}
		
		std::cout << "Case #" << (i + 1) << ":" << std::endl;
		
		// std::cout << h << w << d;
		// std::cout << std::endl;
		ans = solver.solve(N, s);
		for(int j = 0; j < 2; ++j) {
			if(ans[0].empty()) {
				std::cout << "Impossible" << std::endl;
				break;
			}
			std::set<int>::iterator it = ans[j].begin();
			std::cout << *it;
			++it;
			for(; it != ans[j].end(); ++it) {
				std::cout << " " << *it;
			}
			std::cout << std::endl;
		}
	}
	
	return 0;
}

