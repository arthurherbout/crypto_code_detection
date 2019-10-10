#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::ifstream input;
  std::ofstream output;
  uint64_t T;
  input.open("input.in");
  output.open("output.out");
  input >> T;
  for(uint16_t count=1; count<=T; count++) {
    uint64_t N, c=0;
    input >> N;
    std::vector <char> A(N*N);
    std::vector <uint64_t> E(N);
    for(uint64_t i=0; i<N*N; i++) input >> A[i];
    for(uint64_t i=0; i<6; i++) {
      for(uint64_t m=0; m<N; m++) {
        for(uint64_t w=0; w<N; w++) {
          if(A[m+N*w]=='1') {
            if(E[m]==0) E[m]=1;
            for(uint64_t m2=m+1; m2<N; m2++) {
              if(A[m2+N*w]=='1') E[m2]=m;
            }
            for(uint64_t m2=m+1; m2<N; m2++) {
              if(A[m2+N*w]=='0' && E[m2]==m) c++;
            }
          }
        }
      }
    }
    for(uint64_t i=0; i<N; i++) if(E[i]==0) c++;
    output << "Case #" << count << ": ";
    output << c;
    output << '\n';
  }
}
