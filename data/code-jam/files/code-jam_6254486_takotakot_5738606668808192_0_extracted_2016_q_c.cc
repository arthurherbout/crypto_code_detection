#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<set>
#include<iterator>
#include<cmath>
#include<gmpxx.h>

struct solver_2016_q_c {
  int N;
  std::vector<long> primes;

  void prepare(long max) {
    // int arr[max + 1];
    // TODO: http://d.hatena.ne.jp/yatt/20081230/1230641523
    std::vector<bool> arr(max + 1, true);
    long sqrtn = std::sqrt(max) + 1;

    for(int i = 0; i < max; ++i) {
      arr[i] = true;
    }
    arr[0] = arr[1] = false;

    // 2
    primes.push_back(2);
    // std::cerr << 2 << std::endl;
    for(int j = 4; j < max; j += 2) {
      arr[j] = false;
    }

    for(int i = 3; i < sqrtn; i += 2) {
      if(arr[i]) {
        // primes.push_back(i);
        // std::cerr << i << std::endl;
        for(int j = i * 3; j < N; j += i) {
          arr[j] = false;
        }
      }
    }
    for(int i = 3; i < max; i += 2) {
      if(arr[i]) {
        primes.push_back(i);
        // std::cerr << i << std::endl;
      }
    }
  }

  std::string run(int N_, int J) {
    N = N_;

    std::stringstream ss;
    long long max = 1L << (N-2), bits;
    mpz_class num, num_base10;
    std::vector<mpz_class> divisors(11);
    bool all_not_prime;
    int found = 0;

    for(bits = 0; bits < max; ++bits) {
      /*
      // only for test
      if(bits == 2) {
        bits = 12;
      }
      */
      num_base10 = n_in_base(bits, 10);
      std::cerr << "try: " << num_base10 << std::endl;
      // std::cerr << n_in_base(bits, 2) << std::endl;

      all_not_prime = true;
      for(int base = 2; base <= 10; ++base) {
        num = n_in_base(bits, base);
        if(mpz_probab_prime_p(num.get_mpz_t(), 15) > 0) {
          all_not_prime = false;
          break;
        }
        divisors[base] = get_nontrivial_divisor(num);
        if(divisors[base] == 0) {
          std::cerr << "It's difficult to find divisor of " << num << " while considering in base " << base << std::endl;
          all_not_prime = false;
          break;
        }
      }
      if(!all_not_prime) {
        continue;
      }
      std::cerr << "found " << found << " " << num_base10 << std::endl;
      ss << num_base10;
      for(int base = 2; base <= 10; ++base) {
        ss  << " " << divisors[base];
      }
      ss << std::endl;
      ++found;
      if(found == J) {
        break;
      }
    }

    return ss.str();
  }

  mpz_class get_nontrivial_divisor(mpz_class num) {
    std::vector<long>::iterator it;
    for(it = primes.begin(); it < primes.end(); ++it) {
      if((num % (*it)) == 0) return (*it);
    }
    /*
    --it;
    mpz_class divisor_candidate = (*it);
    while(divisor_candidate * divisor_candidate < num) {
      if((num % divisor_candidate) == 0) return divisor_candidate;
      mpz_nextprime(divisor_candidate.get_mpz_t(), divisor_candidate.get_mpz_t());
    }
    */
    std::cerr << "error" << std::endl;
    return 0;
  }

  mpz_class n_in_base(long long bits, int base) {
    int bits_needed = N -2;
    mpz_class num, pow;
    long long bit_flag = 1;
    /*
    if(base == 2) {
      // cannot use ll
      return mpz_class((long long)(1L << (bits_needed - 1)) + (bits << 1) + 1);
    }
    */
    num = 1;
    pow = base;
    for(int i = 0; i < bits_needed; ++i) {
      // std::cerr << i << std::endl;
      // std::cerr << pow << std::endl;
      if((bits & bit_flag)) {
        num += pow;
      }
      bit_flag <<= 1;
      pow *= base;
    }
    num += pow;
    return num;
  }

};

int main(void) {
  solver_2016_q_c solver;

  int T, N, J;

  std::cin >> T;

  // 28 is max for memory
  solver.prepare(1 << 27);
  // return 0;

  for(int i = 0; i < T; ++i) {
    std::cin >> N >> J;
    std::cout << "Case #" << (i+1) << ":" << std::endl;
    std::cout << solver.run(N, J);  // << std::endl;
  }

  return 0;
}
