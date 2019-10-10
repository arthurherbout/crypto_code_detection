#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

pair<pair<long long,long long>,long long> FindLongestChain(vector<long long> & stops);

int main()
{
	ifstream infile("A-small-attempt1.in");
	ofstream outfile("ProblemAOutput.txt");

	int T;
	infile >> T;

	long long N,M;
	long long o,e,p;
	for (int case_num = 1; case_num <= T; case_num++) {
		infile >> N >> M;
		vector<long long> stops(N);
		long long loss = 0;
		long long passengers = 0;

		for (int i = 0; i < M; i++) {
			infile >> o >> e >> p;
			for (int j = (int) o; j < (int) e; j++)
				stops[j] += p;
			loss -= p*(e-o)*(e-o-1)/2;
			passengers += p;
		}

		long long start,end;
		long long people;
		while (passengers > 0) {
			pair<pair<long long,long long>,long long> longest_chain = FindLongestChain(stops);
			start = longest_chain.first.first;
			end = longest_chain.first.second + 1;
			people = longest_chain.second;

			if (start == 0) 
				break;

			loss += people*(end-start)*(end-start-1)/2;

			for (int j = (int) start; j < (int) end; j++)
				stops[j] -= people;
			passengers -= people;
		}

		outfile << "Case #" << case_num << ": " << loss << endl;
	}

	return 0;
}

pair<pair<long long,long long>,long long> FindLongestChain(vector<long long> & stops)
{
	long long n = stops.size();
	long long count = 0;
	long long max_count = 0;
	long long max_passengers = 0;

	long long start = 1;
	long long end = 1;
	
	pair<int,int> max_start_and_end;
	long long num_passengers = 0;


	while (start < n && end < n) {
		while (start < n && stops[start] == 0)
			start++;
		if (start == n)
			break;
		end = start;
		num_passengers = stops[end];
		while (end < n && stops[end] != 0) {
			num_passengers = min(num_passengers, stops[end]);
			end++;
		}
		count = end - start;

		if (count > max_count) {
			max_start_and_end = make_pair(start,end-1);
			max_count = count;
			max_passengers = num_passengers;
		}
		start = end;
	}

	return make_pair(max_start_and_end, max_passengers);
}