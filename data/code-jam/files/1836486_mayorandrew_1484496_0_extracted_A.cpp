#include <iostream>
#include <fstream>

#define SMALL
//#define DEBUG

using namespace std;

int S;
int *var;
bool working=true;


void increase(int k=0){
    if(S-k>=1){
        var[S-k-1]++;
        if(var[S-k-1]==3){
            var[S-k-1]=0;
            increase(k+1);
        }
    }else{
        working=false;
    }
}

int main(){
    #ifndef DEBUG
        #ifdef SMALL
            ifstream input("input1.txt");
            ofstream output("output1.txt");
        #else
            ifstream input("input2.txt");
            ofstream output("output2.txt");
        #endif
    #endif

    int N;
    input >> N;
    for(int i=0; i<N; i++){
        input >> S;
        if(i>0){
            delete[] var;
        }
        var = new int[S];
        int numbers[S];
        for(int k=0; k<S; k++){
            var[k]=0;
            input >> numbers[k];
        }
        working=true;
        output << "Case #" << (i+1) << ": " << endl;
        while(working){
            int S1=0;
            int S2=0;
            int N1=0;
            int N2=0;
            for(int k=0; k<S; k++){
                if(var[k]==1){
                    S1+=numbers[k];
                    N1++;
                }else if(var[k]==2){
                    S2+=numbers[k];
                    N2++;
                }
            }
            if(S1==S2 && N1>0 && N2>0){
                for(int k=0; k<S; k++){
                    if(var[k]==1){
                        output << numbers[k] << " ";
                    }
                }
                output << endl;
                for(int k=0; k<S; k++){
                    if(var[k]==2){
                        output << numbers[k] << " ";
                    }
                }
                output << endl;
                break;
            }
            increase();
        }
        if(!working){
            output << "Impossible" << endl;
        }
    }

    #ifndef DEBUG
        output.close();
        input.close();
    #endif
    return 0;
}
