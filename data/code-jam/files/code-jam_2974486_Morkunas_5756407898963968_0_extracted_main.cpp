#include <iostream>
#include <fstream>
using namespace std;
int T,a, crt,suma = 0,b;
int mas[5][5], masB[5][5];
int main()
{

    ifstream input("A-small-attempt0.in");
    ofstream out("magic.out");

    input >> T;
    for(int t = 1; t<=T; t++){

        input >> a;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                input >> mas[i][j];



        input >> b;

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                input >> masB[i][j];

        for(int i = 0; i < 4; i++){

            for(int j = 0; j < 4; j++){

                if(masB[b-1][i]== mas[a-1][j]){
                    suma++;
                }
            }

        }
        if(suma>1){

            out << "Case #" << t << ": Bad magician!" << endl;
        }
        else if(suma ==0 ) out << "Case #" << t << ": Volunteer cheated!" << endl;

        else
        {
            for(int i = 0; i < 4; i++){

            for(int j = 0; j < 4; j++){

                if(masB[b-1][i]== mas[a-1][j]){

                    out << "Case #" << t << ": " << masB[b-1][i]<< endl;
                }
            }

        }

        }
    suma = 0;

    }



    return 0;
}
