#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

double c,x,f;

double getVal(double farms){
    double val = 0;
    for(double i = 0; i < farms - 1; i++){
        val += (double) (c /(2 + i * f));        
        //cout << val << endl;
    }
    val += (double) (x / (2 + (farms - 1) * f ));
    //cout << val << endl;
    return val;
}

int main(){

    int T;
    int farms = 2;
    double ans1, ans2;
    ifstream fin ("inputtwo");
    ofstream fout("outputtwo");
    fin >> T;
    
    for(int o = 0; o < T; o++){
        fin >> c >> f >> x;
        farms = 2;
        ans1 = x / 2.0;
        ans2 = c / 2.0 + x / (2 + f);
        
        //cout<< o << " " << farms << " " << ans1 << " " << ans2 << endl;
        
        while (ans1 >= ans2){

            farms ++;
            ans1 = ans2;
            ans2 = getVal(farms);
            //cout<< o << " " << farms << " " << ans1 << " " << ans2 << endl;
        }
        
        if (ans1 < ans2){ fout << "Case #" << o + 1 << ": ";
            fout << std::fixed << std::setprecision(7) << ans1 << endl;
            printf("%.10g\n", ans1);
            //cout << endl;
        }
        else{ cout<< "Case #" << o + 1<< ": ";
            fout << std::fixed << std::setprecision(7) << ans2 << endl;
            printf("%.10g\n", ans2);
            //cout << endl;
        }
    }
    

    return 0;
}