#include <iostream>
#include <fstream>
using namespace std;
int suma = 0,T,A,B,K,c;
int main()
{
    ifstream input ("B-small-attempt0.in");
    ofstream out ("fail.out");
    input >> T;
    for(int t = 1; t <=T; t++){
        suma = 0;
        input >> A >> B >> K;
        for(int a = 0 ; a <A; a++){

            for(int b = 0; b <B;b++){
                    c=a&b;
                if(c <K) suma++;
            }

        }
        out << "Case #" << t << ": " << suma << endl;
    }
    return 0;
}
