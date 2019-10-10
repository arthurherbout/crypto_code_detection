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
    vector<pii> mas;
int N,K;

const double pi = 3.14159265358979323846;
double calculate(int T){
    pii tmp = mas[T];

    vector<double> newMas;
    for(int i = 0; i < T; i++){
            newMas.push_back(2.0*pi*(double)mas[i].second*(double)mas[i].first);
    }
    sort(newMas.begin(), newMas.end());
    reverse(newMas.begin(), newMas.end());
    double suma = (double)tmp.first*(double)tmp.first*pi + 2*pi*(double)tmp.first*(double)tmp.second;;
    for(int i = 0; i <K-1; i++){
        suma += newMas[i];
    }
    return suma;
}
void solution(){


    input >> N >> K;
    int R,H;
    mas.clear();
    for(int i = 0 ; i < N; i++){
        input >> R >> H;
        mas.push_back(pii(R,H));
    }
    sort(mas.begin(), mas.end());
    double ans = 0;

    for(int i = K-1; i < N; i++){
        ans = max(ans,calculate(i));
    }
    out << fixed << setprecision(9) << ans << endl;
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
