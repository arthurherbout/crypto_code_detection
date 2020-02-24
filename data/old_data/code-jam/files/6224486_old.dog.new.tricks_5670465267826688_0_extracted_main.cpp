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
    string s;

  public:
    string solution;
    void fill(string s_) {s=s_;}
    void getsolution();

};


ulint convert(char a){
    ulint res;
    switch (a){
        case '1':
            res= 0;
            break;
        case 'i':
            res= 1;
            break;
        case 'j':
            res= 2;
            break;
        case 'k':
            res= 3;
            break;
        default:
            throw std::bad_alloc();
            break;
    };
    return res;
}

tuple<int,char> mult(tuple<int,char> a,tuple<int,char> b){
    vector<vector<char>> mat_char={{('1'),('i'),('j'),('k')},
                                        {('i'),('1'),('k'),('j')},
                                        {('j'),('k'),('1'),('i')},
                                        {('k'),('j'),('i'),('1')},
                                       };
    vector<vector<int>> mat_sign={{(1),(1),(1),(1)},
                             {(1),(-1),(1),(-1)},
                             {(1),(-1),(-1),(1)},
                             {(1),(1),(-1),(-1)},
                             };

    char res_c=mat_char[convert(get<1>(a))][convert(get<1>(b))];
    int res_sign=mat_sign[convert(get<1>(a))][convert(get<1>(b))];
    tuple<int,char> res=make_tuple(res_sign*get<0>(a)*get<0>(b),res_c);
    return res;
}

tuple<int,char> mult_string(string s){
    tuple<int,char> res=make_tuple(1,'1');
    for(auto& it: s){
        res=mult(res,make_tuple(1,it));
    }
    return res;
}


bool rec_sol(vector<tuple<int,char,int>> accum,string initial)
{    auto temp=accum;
     auto temp2=temp;
     string s_=initial;
     bool b=false;
     while (s_.size()>0 && b==false)
       { temp2=temp;
         char c= s_.at(0);
         tuple<int,char> newchar=make_tuple(1,c);
         s_.erase(s_.begin());
         for (ulint i=0;i<temp.size();i++){

                tuple<int,char> to_mult=make_tuple(get<0>(temp.at(i)),get<1>(temp.at(i)));
                tuple<int,char> curr=mult(to_mult,newchar);
                int curr_sign=get<0>(curr);
                char curr_char=get<1>(curr);
                int step =get<2>(temp.at(i));

                if (curr_sign==-1){temp2.at(i)=make_tuple(curr_sign,curr_char,step);}
                else
                  { if (step==0 && curr_char=='i')

                    {
                        temp2.at(i)=make_tuple(curr_sign,curr_char,step);
                        temp2.push_back(make_tuple(1,'1',step+1));
                    }
                    else
                     {if (step==1 && curr_char=='j')
                      {temp2.at(i)=make_tuple(curr_sign,curr_char,step);
                       if (mult_string(s_)==make_tuple(1,'k'))
                            {b=true;
                             break;}}
                      else temp2.at(i)=make_tuple(curr_sign,curr_char,step);

                    }
                  }

         }
         temp=temp2;

        }
  return b;
 }



void ToSolve::getsolution() {

        vector<tuple<int,char,int>> res={make_tuple(1,'1',0)};
        //cout<<s<<endl;

        bool b=false;
        //rec_sol(res,s);
       if (mult_string(s)==make_tuple(-1,'1')) {b=rec_sol(res,s);}
        solution=( (b) ? "YES" : "NO");

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
        string line0;
        getline(infile,line0);
        string line1;
        getline(infile,line1);
        split(line0,' ',v);
        int x=stoi(v.at(1));
        string s_="";
        for (int i=0;i<x;i++){s_=s_+line1;}
        it.fill(s_);

    };
    // where we compute the solutions
    //one one thread:
        //for (auto& it : cases) it.getsolution();
    //in par
    computePar(cases,min({(long unsigned int)7 , n}));
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



