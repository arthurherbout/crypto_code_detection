#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <time.h>
#include <map>
#include <thread>

#define ulint unsigned long int
using namespace std;



//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//Helpers:

//simple split according to a delimitor.
std::vector<std::string> &split(const string &s, char delim, vector<string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


template< class T >

void display_vect(vector <T> v) {
    for(auto& it : v) {
        cout << it <<" ";
    };
    cout << endl;
}


//Simple check if s is in v
bool is_in(vector<ulint> v,ulint s){
    return (find (v.begin(),v.end(),s) != v.end());
}

// /Helpers.
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************


class ToSolve{
  private:
    int r;
    int c;
    int w;

  public:
    string solution;
    void fill(int r_,int c_,int w_) {r=r_;c=c_;w=w_;}
    void getsolution();

};






void ToSolve::getsolution() {

    int line_iter=(c+w-1)/w+w-1;

    int res=line_iter*r;

    solution=to_string(res);

}



// To Parallelize the tests cases:
void compute_atom(vector<ToSolve> &v,ulint n_core,ulint curr){
    for(ulint i=0;i<v.size();i++){
        if ((i % n_core) == curr)(v.at(i)).getsolution();
        if (curr==0 && i % max({(ulint) 1,(v.size()/10)})==0 )
                                    cout<< ((i*100)/v.size())<<"% done"<<endl;
    }
}

void computePar(vector<ToSolve> &v,ulint n_core){
    vector<thread> threads;
    for(ulint i = 0;i<n_core;i++){
        threads.push_back(thread(compute_atom,ref(v),n_core,i));
    }
    for(auto & it : threads){
        it.join();
    }
}


int main()
{
    string n_str;
    ulint n;
    time_t timer,timer_fin;
    time(&timer);
    ifstream infile;
    ofstream outfile;
    
    infile.open("test.in");
    outfile.open("test.out");
    
    getline(infile,n_str);
    n=stoul(n_str);

    cout << n << endl;

    //

    vector<ToSolve>cases(n);
    // where we parse the problems:
    for (auto& it : cases){
        vector<string> v;
        string line1;
        getline(infile,line1);
        split(line1,' ',v);

        it.fill(stoi(v[0]),stoi(v[1]),stoi(v[2]));

    };
    // where we compute the solutions
    //one one thread:
        for (auto& it : cases) it.getsolution();
    //in par
    //computePar(cases,min({(long unsigned int)7 , n}));
    // the above could be parellised


    //we save the cases solutions
    int i = 0;
    for (auto& it : cases){
        outfile << "Case #" << i+1 <<": ";
        outfile << it.solution << endl;
        i++;
    };

    infile.close();
    outfile.close();
    time(&timer_fin);
    cout<<"Done in "<<difftime(timer_fin,timer)<<" s"<<endl;

    return (0);
}



