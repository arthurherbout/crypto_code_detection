#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<iterator>
#include<algorithm>
#include<functional>

typedef std::vector<std::set<long long> > ans_type;
typedef std::map<long long, std::set<long long> > sum_type;
typedef std::map<int, int> v_list_type;
typedef std::map<int, std::map<int, int> > v_type;
// typedef std::map<int, std::map<int, std::vector> > v_type;
class DiamondInheritance;

class DiamondInheritance{
public:
	static const double eps = 0.000000001;
	
	DiamondInheritance(){}
	std::string solve(int classes, v_type v) {
		bool flag = true;
		v_type reachable_old, reachable_new;
		
		reachable_old = v;
		
		// dump
		std::cerr << std::endl;
		for(int i = 1; i <= classes; ++i) {
			for(int j = 1; j <= classes; ++j) {
				if(reachable_old[i][j]) {
					std::cerr << "1 ";
				}else{
					std::cerr << "0 ";
				}
			}
			std::cerr << std::endl;
		}
		
		for(int loop = 0; loop < classes; ++loop) { // while flag
			flag = false;
			for(int i = 1; i <= classes; ++i) {
				for(v_list_type::iterator it = reachable_old[i].begin(); it != reachable_old[i].end(); ++it) {
					if(!it->second) { // false
						continue;
					}
					reachable_new[i][it->first] = 1;
					// i to it->first
					for(v_list_type::iterator it2 = reachable_old[it->first].begin(); it2 != reachable_old[it->first].end(); ++it2) {
						// it->first to it2->first
						if(!it2->second) { // false
							continue;
						}
						// if(reachable_old[i]) {
						// }
						reachable_new[i][it2->first] = 1;
					}
				}
			}
			reachable_old = reachable_new;
		}
		
		// dump
		/*
		std::cerr << std::endl;
		for(int i = 1; i <= classes; ++i) {
			for(int j = 1; j <= classes; ++j) {
				if(reachable_old[i][j]) {
					std::cerr << "1 ";
				}else{
					std::cerr << "0 ";
				}
			}
			std::cerr << std::endl;
		}
		*/
		
		// check
		for(int i = 1; i <= classes; ++i) {
			for(int j = 1; j <= classes; ++j) {
				if( reachable_new[i][j] ) {
					// to check
					int cnt = 0;
					for(int k = 1; k <= classes; ++k) {
						if( v[i][k] ) {
							if( reachable_new[k][j] ) {
								cnt++;
							}else if( k == j){
								cnt++;
							}
						}
					}
					if(cnt >= 2) {
						return "Yes";
					}
				}
			}
		}
		
		
		
		return "No";
	}
};

int main(void) {
	int n_probs = 0;
	DiamondInheritance solver;
	
	std::cin >> n_probs;
	int classes;
	// std::vector<long long> s;
	// std::vector<double> ans;
	std::vector<int> i;
	v_type v;
	
	for(int i = 0; i < n_probs; ++i) {
		std::cin >> classes;  // 1 to classes
		v.clear();
		int num_vs, v_from;
		
		for(int j = 1; j <= classes; ++j) {
			std::cin >> num_vs;
			for(int k = 0; k < num_vs; ++k) {
				std::cin >> v_from;
				v[v_from][j] = 1;
				std::cerr << v_from << j << std::endl;
			}
		}
		
		std::cout << "Case #" << (i + 1) << ": ";
		std::cout << solver.solve(classes, v) << std::endl;
	}
	
	return 0;
}

