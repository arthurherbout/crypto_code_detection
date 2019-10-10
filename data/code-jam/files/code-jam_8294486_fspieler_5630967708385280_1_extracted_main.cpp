#include <iostream>
#include <string>
#include <cstdio>
#include <vector> 
#include <algorithm> 
#include <tuple>

#include <includes.h>

using namespace std;

string run_test_case()
{

    int d, n;
    cin >> d >> n;
    vector<double> horse_times;
    double max_time = 0;
    while(n-->0){
        int ki, si;
        cin >> ki >> si;
        double time = ((double)(d-ki))/si;
        if (time > max_time){
            max_time = time;
        }
    }
    char ret[100];
    sprintf(ret, "%f", ((double)d)/max_time);
    return toString(ret);
}

int main(int argc, char* argv[])
{

    int num_test_cases;
    int start;
    if(argc == 3)
    {
        num_test_cases = atoi(argv[1]);
        start = atoi(argv[2]);
    }
    else
    {
        cin >> num_test_cases;
        start = 1;
    }

    for (int test_case = start; test_case < start+num_test_cases; test_case++)
    {
        cout << "Case #" << test_case << ": " << run_test_case() << endl;
    }

    return 0;
}
