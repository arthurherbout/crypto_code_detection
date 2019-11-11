#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;
const int MAXN = 1000+5;
const double oo = numeric_limits<double>::max();

int T, dydis,suma = 0,save1=0;
char a;
int sveikoji_dalis, trupmenine_dalis;
double skaicius;
double masL[MAXN],masL1[MAXN], masP[MAXN], masP1[MAXN];

int main()
{
    ifstream input("D-large.in");
    ofstream out ("kazkas.out");

    input >> T;

    for(int t = 1; t<=T; t++){

        input >> dydis;
        for(int i = 0; i < dydis; i++){

                input >> skaicius;


                masL[i]=skaicius;
                masL1[i]=skaicius;
        }
        for(int i = 0; i < dydis; i++){
                input >> skaicius;
                masP[i]=skaicius;
                masP1[i]=skaicius;

        }
        sort(masP, masP+dydis);
        sort(masL, masL+dydis);
        for(int i =0; i < dydis; i++){

            for(int y = 0; y < dydis; y++){
              //  cout << masL[i] << " " << masP[y] << endl;
                if(masL[i]<masP[y]){
                    suma++;
                    masP[y]=-oo;
                    break;

                }
            }


        }

        sort(masP1, masP1+dydis);
        sort(masL1, masL1+dydis);

       /* for(int i = 0 ; i < dydis; i++) cout << masL1[i] << endl;
        cout << endl;
        for(int i = 0 ; i < dydis; i++) cout << masP1[i] << endl;*/


        int st = 0, fn = dydis-1;
        int stL = 0, stP = 0, fnP = dydis-1;
        while(stL<dydis){

                if(masL1[stL]>masP1[stP]){
                    stL++; stP++;
                    save1++;
                }
                else{
                    stL++;
                    fnP--;
                }

        }


        out << "Case #" << t << ": " << save1 <<  " " << dydis - suma << endl;
suma = 0;
save1=0;

    }


    return 0;
}
