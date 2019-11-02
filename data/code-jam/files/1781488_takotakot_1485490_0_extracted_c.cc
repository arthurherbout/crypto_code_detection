#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<iterator>
#include<algorithm>
#include<functional>

typedef long long ll;
typedef std::vector<std::set<long long> > ans_type;
typedef std::map<long long, std::set<long long> > sum_type;
typedef std::map<int, int> v_list_type;
typedef std::map<int, std::map<int, int> > v_type;
// typedef std::map<int, std::map<int, std::vector> > v_type;
typedef std::pair<ll, int> num_box_type;
class DiamondInheritance;

class DiamondInheritance{
public:
	static const double eps = 0.000000001;
	
	std::vector<num_box_type> boxlist, toylist;
	int N, M;
	
	DiamondInheritance(){}
	ll dfs(int ibox, int itoy, ll sc, int now_type, ll remain) {
		int box_index = 0, toy_index = 0;
		bool box_find, toy_find, to_search;;
		
		ll sc_max = sc, sc_cur = sc;
		
		for(int p_type = 1; p_type <= 100; ++p_type) {
//			std::cerr << p_type << std::endl;
			// toy and box to use
			int j;
			to_search = false;
			if(p_type != now_type) {
				j = ibox + 1;
				to_search = true;
			}else if(remain <= 0) {  // type is same
				j = ibox + 1;
				to_search = true;
			}else{
				box_index = ibox;
			}
			if(to_search) {
				box_find = false;
				for(; j < N; ++j) {
//					std::cerr << j << " ";
					if(boxlist[j].second == p_type) {
						box_index = j;
						box_find = true;
						break;
					}
				}
				if(!box_find) {
					continue;
				}
			}
			
			to_search = false;
			if(p_type != now_type) {
				j = itoy + 1;
				to_search = true;
			}else if(remain >= 0) {  // type is same
				j = itoy + 1;
				to_search = true;
			}else{
				toy_index = itoy;
			}
			if(to_search) {
				toy_find = false;
				for(; j < M; ++j) {
					if(toylist[j].second == p_type) {
						toy_index = j;
						toy_find = true;
						break;
					}
				}
				if(!toy_find) {
					continue;
				}
			}
			// std::cerr << "match: " << box_index << " " << toy_index << std::endl;
			
			if(p_type != now_type) {
//				std::cerr << "type " << 1 << std::endl;
				sc_cur = sc + std::min(boxlist[box_index].first, toylist[toy_index].first);
				remain = boxlist[box_index].first - toylist[toy_index].first;
				sc_max = std::max(sc_max, dfs(box_index, toy_index, sc_cur, p_type, remain));
			}else if(remain == 0) {
//				std::cerr << "type " << 2 << std::endl;
				sc_cur = sc + std::min(boxlist[box_index].first, toylist[toy_index].first);
				remain = boxlist[box_index].first - toylist[toy_index].first;
				sc_max = std::max(sc_max, dfs(box_index, toy_index, sc_cur, p_type, remain));
			}else if(remain > 0) {
//				std::cerr << "type " << 3 << std::endl;
				sc_cur = sc + std::min(remain, toylist[toy_index].first);
				remain = remain - toylist[toy_index].first;
				sc_max = std::max(sc_max, dfs(box_index, toy_index, sc_cur, p_type, remain));
			}else{
//				std::cerr << "type " << 4 << std::endl;
				sc_cur = sc + std::min(boxlist[box_index].first, -remain);
				remain = boxlist[box_index].first + remain;
				sc_max = std::max(sc_max, dfs(box_index, toy_index, sc_cur, p_type, remain));
			}
		}
		return sc_max;
	}
	ll solve(int N_, int M_, std::vector<num_box_type> boxlist_, std::vector<num_box_type> toylist_) {
		N = N_;
		M = M_;
		boxlist = boxlist_;
		toylist = toylist_;
		// std::cerr << "called" << std::endl;
		return dfs(-1, -1, 0, 0, 0);
	}
};

int main(void) {
	int n_probs = 0;
	DiamondInheritance solver;
	
	std::cin >> n_probs;
	int N, M;
	// std::vector<long long> s;
	// std::vector<double> ans;
	std::vector<int> i;
	v_type v;
	num_box_type l;
	std::vector<num_box_type> boxlist, toylist;
	//std::cerr << n_probs << std::endl;
	for(int i = 0; i < n_probs; ++i) {
		//std::cerr << i << std::endl;
		boxlist.clear();
		toylist.clear();
		std::cin >> N >> M;
		
		for(int j = 0; j < N; ++j) {
			std::cin >> l.first >> l.second;
			boxlist.push_back(l);
//			std::cerr << l.first << " ";
		}
//		std::cerr << std::endl;
		for(int j = 0; j < M; ++j) {
			std::cin >> l.first >> l.second;
			toylist.push_back(l);
//			std::cerr << l.first << " ";
		}
		
		std::cout << "Case #" << (i + 1) << ": ";
		std::cout << solver.solve(N, M, boxlist, toylist) << std::endl;
	}
	
	return 0;
}

