#include <iostream>
#include <string>
#include <cstdio>
#include <vector> 
#include <algorithm> 


using namespace std;

long long firstMethod(vector<long long> &samples);
long long secondMethod(vector<long long> &samples);

int main(){

    int num_test_cases;
    cin >> num_test_cases;
    for(int test_case = 1; test_case <= num_test_cases; ++test_case){

        int num_samples;
        cin >> num_samples;
        vector<long long> samples;
        for(int s = 0; s < num_samples; s++){
            long long temp;
            cin >> temp;
            samples.push_back(temp);
        }

        cout << "Case #" << test_case << ": " << firstMethod(samples) << " " << secondMethod(samples) << endl;
    }
    return 0;
}

long long firstMethod(vector<long long> &samples){
    long long total = 0;
    long long last = 0;
    for(long long s: samples){
        if(s < last){
            total += last - s;
        }
        last = s;
    }
    return total;
}

long long secondMethod(vector<long long> &samples){
    long long max_diff = 0;
    long long last = samples[0];
    for(auto s: samples){
        max_diff = max(max_diff, last-s);
        last = s;
    }
    long long total = 0;
    for(int s = 0; s < samples.size() - 1; s++){
        total += min(samples[s],max_diff);
    }
    return total;

}
