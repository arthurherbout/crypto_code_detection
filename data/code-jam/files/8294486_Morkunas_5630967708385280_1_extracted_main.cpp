#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;
string file_name = "A-large";

int gcd(int a, int b){
    while(a&&b){
        if(a>b) a%=b;
        else b%=a;
    }
    return a+b;
}

ifstream input(file_name + ".in");
ofstream out(file_name +".out");
typedef pair<int,int> pii;


//#define input cin
//#define out cout
const int MAXN = 1000 + 5;
void solution(){
    long long  N;
    double D, K, S, ans = 0.0;
    //double mas,[MAXN];
    input >> D >> N;
    for(int i = 0; i < N; i++){
        input >> K >> S;
        ans = max((D-K)/S, ans);
    }
    double result = D/ans;
    out << fixed <<  setprecision(6) << result << endl;;
}

int main()
{
    int T;
    input >> T;
    for(int c = 0; c < T; c++){
        out << "Case #" << c+1 << ": ";
        solution();
    }
    return 0;
}
