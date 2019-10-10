#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<sstream>

using namespace std;

    int domain,range;
void FairAndSquare(ifstream &fin,ofstream &fout,int globalCounter){
     
     fin >> domain >> range;
     int z;
     for(z=domain;z<=range;z++){
             bool doubleIntSame=false;
             bool squareFair=false;
             bool fair=false;
             ostringstream convert;
             convert <<z;
             string f=convert.str();
             int m=f.length();
             m=m-1;
             string l;
             for(int i=0;i<m;i++){
                     l[i]=f[m-i];
                     
                     }
             int counter=0;
             for(int i=0;i<m;i++){
                     if(l[i]==f[i]){
                          counter++;
                     }       

              }            
             if(counter==m){
                            fair=true;
                            }         
             else fair =false;          
             
             
             if(fair){
                      int temp2;
                      double temp;
                      temp=sqrt(z);
                      temp2=temp;
                      if(temp2>=1 && temp2==temp){
                            doubleIntSame=true;
                      }
                      if(doubleIntSame){
                               ostringstream convert;
                               convert <<temp2;
                               string f=convert.str();
                               int m=f.length();
                               m=m-1;
                               string l;
             for(int i=0;i<m;i++){
                     l[i]=f[m-i];
                     
                     }
                     int counter=0;
             for(int i=0;i<m;i++){
                     if(l[i]==f[i]){
                          counter++;
                     }       

}                     if(counter==m){
                                    squareFair=true;
                                    }         
                                    else squareFair =false;          
                                                              
                               
                               }
              
             
if((fair) && (squareFair)){
        globalCounter++;
        }

             
             }

}


fout<< globalCounter << endl;


}
     
     
     
int main(){
    int T;    
    string temp;
    ifstream fin;
    ofstream fout;
    fout.open("output.txt");
    fin.open("C-small-attempt0.in");
    fin >>T;
    int globalCounter=0;
    for(int i=0;i<T;i++){
            fout<< "Case #"<<i+1<<": " ;
            FairAndSquare(fin,fout,globalCounter);
//            getline(fin,temp);
            }

    
    
    
    
    
    
    system("pause");
    return 0;
    }
