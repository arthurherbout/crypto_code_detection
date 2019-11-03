#include<iostream>
#include<fstream>
#include<string>

using namespace std;

    char tic[4][4];

void TicTacToeTOMEK(ifstream &fin,ofstream &fout){
     
    for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
    {
            fin>>tic[i][j];
    }     

     bool playerX=false;
     bool playerY=false;
     bool draw=false;
     bool gameOver=false;
     int counter =0;

     for(int i=0;i<4;i++){
     for(int j=0;j<4;j++){
             if(tic[i][j]=='X' || tic[i][j]=='T'){
                                counter++;
                                if(counter==4){
                                               playerX=true;
                                               }
                                
                                }
                                }
                                counter=0;
                                }
     for(int i=0;i<4;i++){
     for(int j=0;j<4;j++){
             if(tic[j][i]=='X' || tic[j][i]=='T'){
                                counter++;
                                if(counter==4){
                                               playerX=true;
                                               }
                                
                                }
                                }
                                counter=0;
                                }
     
     for(int i=0;i<4;i++)
             if(tic[i][i]=='X' || tic[i][i]=='T'){
                                counter++;
                                if(counter==4){
                                               playerX=true;
                                               }
                                
                                }
                                counter=0;
     
     for(int i=0;i<4;i++)
             if(tic[i][3-i]=='X' || tic[i][3-i]=='T'){
                                counter++;
                                if(counter==4){
                                               playerX=true;
                                               }
                                
                                }
    counter=0;
    //--------------------------------------
     for(int i=0;i<4;i++){
     for(int j=0;j<4;j++){
             if(tic[i][j]=='O' || tic[i][j]=='T'){
                                counter++;
                                if(counter==4){
                                               playerY=true;
                                               }
                                
                                }
                                }
                                counter=0;
                                }
     for(int i=0;i<4;i++){
     for(int j=0;j<4;j++){
             if(tic[j][i]=='O' || tic[j][i]=='T'){
                                counter++;
                                if(counter==4){
                                               playerY=true;
                                               }
                                
                                }
                                }
                                counter=0;
                                }
     
     for(int i=0;i<4;i++)
             if(tic[i][i]=='O' || tic[i][i]=='T'){
                                counter++;
                                if(counter==4){
                                               playerY=true;
                                               }
                                
                                }
                                counter=0;
     
     for(int i=0;i<4;i++)
             if(tic[i][3-i]=='O' || tic[i][3-i]=='T'){
                                counter++;
                                if(counter==4){
                                               playerY=true;
                                               }
                                
                                }
           
    //--------------------------------------
          if((!playerX) && (!playerY)){
          counter=0;
                      
                      for(int i=0;i<4;i++){
                              for(int j=0;j<4;j++){
                                      if(tic[i][j]=='.')
                                                        counter++;             
                                      }
                                      }
                      
                      
                      if(counter>=1){
                                     gameOver=true;
                                     }
                      /*else if(counter=0)
                                        draw=true;
                      */
                      
                      } 
          
          
          
          
    //----------------------------------------
             if(playerX){
                         fout<< "X won" <<endl;
                         }
             else if(playerY){
                         fout<< "O won" <<endl;
                         }
             else if(gameOver){
                         fout<< "Game has not completed" <<endl;
                         }
             else 
                  fout<< "Draw" <<endl;
            
     
     
     
}
     
     
     
int main(){
         
    int T;
    string temp;
    ifstream fin;
    ofstream fout;
    fout.open("output.txt");
    fin.open("A-large.in");
    fin >>T;
    for(int i=0;i<T;i++){
            fout<< "Case #"<<i+1<<": " ;
            TicTacToeTOMEK(fin,fout);
            getline(fin,temp);
            }
            
     
             
         system("pause");
         return 0;         
         
}
