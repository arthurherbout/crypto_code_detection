#include <iostream>
#include <string>
#include <cstdio>
#include <vector> 
#include <algorithm> 


using namespace std;

int firstMethod(vector<int> &samples);
int secondMethod(vector<int> &samples);

int main(){

    int num_test_cases;
    cin >> num_test_cases;
    for(int test_case = 1; test_case <= num_test_cases; ++test_case){

        int num_samples;
        cin >> num_samples;
        vector<int> samples;
        for(int s = 0; s < num_samples; s++){
            int temp;
            cin >> temp;
            samples.push_back(temp);
        }

        cout << "Case #" << test_case << ": " << firstMethod(samples) << " " << secondMethod(samples) << endl;
    }
    return 0;
}

int firstMethod(vector<int> &samples){
    int total = 0;
    int last = 0;
    for(int s: samples){
        if(s < last){
            total += last - s;
        }
        last = s;
    }
    return total;
}

int secondMethod(vector<int> &samples){
    int max_diff = 0;
    int last = samples[0];
    for(auto s: samples){
        max_diff = max(max_diff, last-s);
        last = s;
    }
    int total = 0;
    for(int s = 0; s < samples.size() - 1; s++){
        total += min(samples[s],max_diff);
    }
    return total;

}
