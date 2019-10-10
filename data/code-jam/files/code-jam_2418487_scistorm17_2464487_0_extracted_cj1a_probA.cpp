/*
 * cj1a_prob1.cpp
 *
 *  Created on: Apr 26, 2013
 *      Author: Matt
 */

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ofstream out;
	out.open("answer.txt");

	int T;

	cin >> T;

	for(int i=1; i<=T; i++)
	{
		long long r, t;

		cin >> r; cin >> t;

		long long paint_used = 0;
		long long ring_count = 0;

		for(long long j=r+1; paint_used <= t; j+=2)
		{
			paint_used += 2*j-1;
			ring_count++;
		}

		ring_count--;

		out << "Case #" << i << ": " << ring_count << endl;
	}

	out.close();
}


