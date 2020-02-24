#include <iostream>
#include <string>
#include <cstdio>
#include <vector> 
#include <algorithm> 


#define I 1
#define J 2
#define K 3

using namespace std;

bool reduce(vector<short>::iterator begin, vector<short>::iterator end);
short special_multiply(short a, short b);

int main(int argc, char* argv[]){

    int start = atoi(argv[1]);
    int num_test_cases = 1;

    for (int test_case = start; test_case < start+num_test_cases; test_case++){
        int L, X;
        cin >> L;
        cin >> X;
        string input;
        cin >> input;
        vector<short> symbols;
        for(int i = 0; i < X; i++){
            for(char& c : input){
                symbols.push_back(c - 'h');
            }
        }
        cout << "Case #" << test_case << ": " << (reduce(symbols.begin(), symbols.end()) ? "YES" : "NO" ) << endl;
    }

    return 0;
}

bool reduce(vector<short>::iterator it, vector<short>::iterator end){
    short curr_val_i = 5;
    while(it != end){
        curr_val_i = special_multiply(curr_val_i, *it);
        if(curr_val_i == I){
            vector<short>::iterator it_j(it+1);
            short curr_val_j = 5;
            while(it_j != end){
                
                curr_val_j = special_multiply(curr_val_j, *it_j);
                if(curr_val_j == J){
                    vector<short>::iterator it_k(it_j+1);
                    short curr_val_k = 5;
                    while(it_k != end){
                        curr_val_k = special_multiply(curr_val_k, *it_k);
                        it_k++;
                    }
                    if(curr_val_k == K){
                        return true;
                    }
                }
                it_j++;
            }
        }
        it++;
    }
    return false;
}

// i->0, j->1, k->2
short special_multiply(short a, short b){

    if(a == 5){
        return b;
    }
    if(a == -5){
        return -b;
    }

    short sign = 1;
    if(a < 0){
        a*=-1;
        sign *= -1;
    }
    if(b < 0){ 
        b*=-1;
        sign *= -1;
    }

    if(a == I){
        if(b == J){
            return sign*K;
        }
        else if(b == K){
            return sign*-J;
        }
    }
    else if(a == J){
        if(b == I){
            return sign*-K;
        }
        else if(b == K){ 
            return sign*I;
        }
    }
    else{ //a == k
        if(b == I){
            return sign*J;
        }
        else if(b == J){
            return sign*-I;
        }
    }
    //a == b
    return sign*-5;
}
