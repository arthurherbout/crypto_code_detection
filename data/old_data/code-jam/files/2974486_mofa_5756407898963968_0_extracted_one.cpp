#include <iostream>
#include <fstream>

using namespace std;

int main(){
    int T;
    int cards[4][4];
    int cards2[4][4];
    int chosen[4];
    int row;
    int matches;
    int ans;
    ifstream myfile ("inputone");
    ofstream fout("outputone");
    if (myfile.is_open())
    {
        myfile >> T;
        for(int o = 0; o < T; o++){
        matches = 0;
        //cout << T << endl;
        myfile >> row;
        //cout << row << endl;
        for(int i = 0; i < 4; i ++){
            for(int j = 0; j < 4; j ++){
                myfile >> cards[i][j];    
                //cout << cards[i][j] << " ";
            }
            //cout << endl;
        }
        for(int j = 0; j < 4; j ++){
            chosen[j] = cards[row - 1][j];   
            //cout << chosen[j] << " " ;  
        }
        //cout << endl;
        
        myfile >> row;
        for(int i = 0; i < 4; i ++){
            for(int j = 0; j < 4; j ++){
                myfile >> cards[i][j];    
            }
        }
        
        for(int i = 0; i < 4; i ++){
            for(int j = 0; j < 4; j ++){
                if(cards[row - 1][i] == chosen[j]){ 
                    matches ++;   
                    ans = chosen[j];
                }
            }
        }
        fout << "Case #" << o + 1 << ": ";       
        if(matches == 1) fout << ans << endl;
        if(matches > 1) fout << "Bad magician!" << endl;
        if (matches == 0) fout << "Volunteer cheated!" << endl;
        }       
        myfile.close();
        fout.close();
    }
    return 0;
}