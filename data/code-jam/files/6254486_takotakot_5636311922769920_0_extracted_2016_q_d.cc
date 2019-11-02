#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<set>
#include<iterator>

struct solver_2016_q_d {
  // use 0-origin
  int K, C, S;
  std::vector<std::vector<long long> > pows;

  std::string run(int K_, int C_, int S_) {
    std::stringstream ss;
    std::string impossible = "IMPOSSIBLE";
    std::vector < std::vector<int> > uses;
    std::vector<int> use;
    K = K_;
    C = C_;
    S = S_;

    if (C * S < K) {
      return impossible;
    }

    prepare_pows();
    int s, k, c, t;
    s = 0;
    t = 0;
    for (s = 0; s < S; ++s) {
      use.clear();
      for (int c = 0; c < C; ++c) {
        use.push_back(t);
        std::cerr << "push " << t << " ";
        ++t;
        if (t == K) {
          break;
        }
      }
      uses.push_back(use);
      std::cerr << "renew" << std::endl;
      if (t == K) {
        break;
      }
    }

    // back to 1
    ss << fractal_counter(uses[0]) + 1;
    std::cerr << fractal_counter(uses[0]) + 1;
    for (int i = 1; i < uses.size(); ++i) {
      std::cerr << " " << fractal_counter(uses[i]) + 1;
      ss << " " << fractal_counter(uses[i]) + 1;
    }
    std::cerr << std::endl;

    return ss.str();
  }

  void prepare_pows() {
    long long num;
    // std::cerr << "begin resize" << std::endl;
    pows.clear();
    pows.resize(K + 1, std::vector<long long>(C + 1));
    // std::cerr << "end resize" << std::endl;
    for (int i = 1; i <= K; ++i) {
      num = 1;
      for (int j = 1; j <= C; ++j) {
        num *= i;
        pows[i][j] = num;
        // std::cerr << i << " " << j << " " << num << std::endl;
      }
    }
    return;
  }

  long long fractal_counter(std::vector<int> arr) {
    long long num = 0, pow = 1;
    for (std::vector<int>::reverse_iterator it = arr.rbegin(); it != arr.rend();
        ++it) {
      num += (*it) * pow;
      pow *= K;
    }
    return num;
  }

};

int main(void) {
  solver_2016_q_d solver;

  int T, K, C, S;

  std::cin >> T;
  for (int i = 0; i < T; ++i) {
    std::cin >> K >> C >> S;
    std::cerr << "Case #" << (i + 1) << ": " << std::endl;
    std::cout << "Case #" << (i + 1) << ": " << solver.run(K, C, S)
        << std::endl;
  }

  return 0;
}
