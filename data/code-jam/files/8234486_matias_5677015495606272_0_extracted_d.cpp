#include<iostream>
using namespace std;

int main() {
  int T;
  cin>>T;
  for (int t=1;t<=T;t++) {
    int R,C;
    cin>>R>>C;
    int res=1+(R%3==0);
    if (C%3==0) {
      if (R==2) res=2;
      if (R==3) res=2;
      if (R==4) res=3;
      if (R==5) res=3;
      if (R==6) res=6;
    }
    cout<<"Case #"<<t<<": "<<res<<endl;      
  }
}
