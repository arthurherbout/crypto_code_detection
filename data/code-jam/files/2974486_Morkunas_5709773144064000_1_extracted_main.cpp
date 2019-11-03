#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;
double greitis = 2.0;
double laikas = 0.0;
double C,F,X,T,saveMin;
typedef pair<double,double> pdd;
const int MAXN = 10000000;
pdd mas[MAXN];

int main()
{
    ifstream input("B-large.in");
    ofstream out("duomenys.out");

    input >> T;

    for(int t = 1; t<=T; t++){

        input >> C >> F >> X;

            mas[0].first=0;
            mas[0].second = X/greitis;
        saveMin = mas[0].second;
    for(int i = 1; i < MAXN; i++){

         mas[i].first= mas[i-1].first + C/(greitis+F*(i-1));
         mas[i].second = mas[i].first + X/(greitis+F*i);
    }
    for(int i = 1; i < MAXN; i++){

        saveMin = min(saveMin, mas[i].second);
    }
   out << "Case #" << t << ": " << fixed<<setprecision(20) << saveMin<< endl;
    }

    return 0;
}
