#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    ifstream infile("A-small-attempt0.in");
    //ifstream infile("ProblemAInput.txt");
    ofstream outfile("ProblemAOutputSmall.txt");

    int T;
    infile >> T;

    for (int test = 0; test < T; test++) {
        int N,L;
        infile >> N >> L;
        vector<int> initial_flows(L);
        vector<int> required_flows(L);

        string initFlow;
        for (int i = 0; i < N; i++) {
            infile >> initFlow;
            for (int j = 0; j < L; j++) {
                if (initFlow[j] == '1') {
                    initial_flows[j] += 1;
                }
            }
        }

        string reqFlow;
        for (int i = 0; i < N; i++) {
            infile >> reqFlow;
            for (int j = 0; j < L; j++) {
                if (reqFlow[j] == '1') {
                    required_flows[j] += 1;
                }
            }
        }

        int flops = 0;
        bool impossible = false;
        for (int j = 0; j < L; j++) {
            if (initial_flows[j] == required_flows[j]) {
                // do nothing
            }
            else if (initial_flows[j] == N - required_flows[j]) {
                flops++;
            }
            else {
                impossible = true;
            }
        }

        if (impossible)
            outfile << "Case #" << (test+1) << ": " << "NOT POSSIBLE" << endl;
        else
            outfile << "Case #" << (test+1) << ": " << flops << endl;
    }

    return 0;
}
