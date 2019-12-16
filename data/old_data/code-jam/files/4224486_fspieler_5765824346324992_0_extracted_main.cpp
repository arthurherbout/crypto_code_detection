#include <iostream>
#include <string>
#include <cstdio>
#include <vector> 
#include <algorithm> 


using namespace std;

int compute(int pos, vector<int>& barber_latencies, vector<int>& times_left);

int main(int argc, char* argv[]){

    int num_test_cases;
    int start;
    cerr << argc << endl;
    if(argc == 3){
        num_test_cases = atoi(argv[1]);
        start = atoi(argv[2]);
    } else {
        cin >> num_test_cases;
        start = 1;
    }
    for(int test_case = start; test_case < start+num_test_cases; ++test_case){
        int num_barbers;
        cin >> num_barbers;
        int pos_in_line;
        cin >> pos_in_line;
        vector<int> barber_latencies;
        for(int i = 0; i < num_barbers; i++){
            int temp;
            cin >> temp;
            barber_latencies.push_back(temp);
        }
        vector<int> times_left(num_barbers, 0);
        cout << "Case #" << test_case << ": " << compute(pos_in_line, barber_latencies, times_left) << endl;
    }
    return 0;
}

int compute(int pos, vector<int>& barber_latencies, vector<int>& times_left){

    int min = *min_element(times_left.begin(), times_left.end());

    int num_barbers = barber_latencies.size();
/*
    for(int b = 1; b <= num_barbers; b++){
        cout << times_left[b-1] << " ";
    }
    cout << endl;
    */
    for(int i = 0; i < num_barbers; i++){
        times_left[i] -= min;
        if(times_left[i] <= 0){
            times_left[i] = barber_latencies[i];
            pos -= 1;
            if(pos == 0){
                return i+1;
            }
        }
    }

    return compute(pos, barber_latencies, times_left);
}
