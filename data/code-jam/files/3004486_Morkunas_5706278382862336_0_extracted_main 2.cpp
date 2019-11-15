#include <iostream>
#include <fstream>
using namespace std;

int DBD(int a, int b){
    while(a&&b){

        if(a>=b)a%=b;
        else b%=a;
    }
    return a+b;
}
int isLaipsnis(int a){

    while(a!=1){
        if(a%2 == 0)
            a/=2;
        else return false;

    }
    return true;
}


int T,P,Q,dbd,A;
bool is;
char ch;
int era = 1;
int main()
{
    ifstream input ("A-small-attempt0.in");
    ofstream out("testas.out");
    input >> T;

    for(int t = 1; t <=T; t++){

        input >> P  >> ch >> Q;
        dbd = DBD(P,Q);
        P/=dbd;
        Q/=dbd;
        is = isLaipsnis(Q);
        era = 1;
        while(true){
        dbd = DBD(P,Q);
        P/=dbd;
        Q/=dbd;
        A = 2*P - Q;
        if(A >=0)break;
        else P*=2;
        era++;
        }
        if(is==false) out << "Case #" << t<< ": " << "impossible" << endl;
        else out << "Case #" << t<< ": " << era << endl;
    }



    return 0;
}

