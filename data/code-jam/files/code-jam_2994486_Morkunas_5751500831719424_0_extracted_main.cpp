#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;


vector <string>vec;
int T,N,suma,ats = 0,middle,a=0,index;
char current;
string str;
bool baigta = false;

int main()
{

    ifstream input ("A-small-attempt1.in");
    ofstream out("failas.out");

    input >> T;

    for(int i = 1; i <=T; i++){
       suma = 0;
       input >> N;
       for(int y = 0 ; y < N; y++){

            input >> str;
           // suma+=str.size();
            vec.push_back(str);


       }

       while(true){

            if(vec[0].size()==0 || baigta==true) break;
       else{
       current = vec[0][0];
    for(int y = 0 ; y < N; y++){
            index = 0;
        if(vec[y][0]!=current) baigta = true;
        else{
            while(vec[y][index]==current){
                index++;
                suma++;
            }
        }
    }
    if(baigta==true) break;
    else{
   // cout << current << " = " <<  suma << endl;
       middle = suma/N;
       if(suma%N > N%2+N/2)middle++;
  //  cout << "vidurkis - " <<  middle << endl;
    suma = 0;


       for(int y = 0; y <N; y++){
            a=0;
            if(vec[y].size()!=0){
            while(vec[y][0]==current){
        vec[y].erase(0,1);
        a++;

       }
            }
            else baigta = true;

            if(middle >= a)
                ats += middle-a;
            else ats += a-middle;
       }




       }
       }
       }
       for(int y = 0 ; y <N; y++)
        if(vec[y].size()!=0) baigta = true;

    if(baigta == true) out << "Case #"<< i <<  ": Fegla Won" << endl;
    else out << "Case #" << i << ": " << ats << endl;

    vec.clear();
    ats= 0;
    baigta = false;
    }


    return 0;
}


