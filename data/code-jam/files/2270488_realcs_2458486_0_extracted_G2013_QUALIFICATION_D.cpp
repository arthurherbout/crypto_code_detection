#define G2013_QUALIFICATION_D 1
#if G2013_QUALIFICATION_D == 1

#ifndef GIN
	#define GIN "D-small-attempt0.in" 
	#define GOUT "D-small-attempt0.out"
#endif

#ifndef GIN
	#define GIN "input.txt" 
	#define GOUT "output.txt"
#endif

#ifndef GIN
	#define GIN "D-large.in" 
	#define GOUT "D-large.out"
#endif





#define myfile(B) ("E:\\CodeJam\\D\\"##B)

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
using namespace std;

ifstream g_infile;
ofstream g_outfile;
#define read(x) {g_infile >> x;}
#define readline(x) {std::getline(g_infile, x);}
#define write(x) {g_outfile << x;}
#define result_head(x) {g_outfile << "Case #"; g_outfile << x;  g_outfile << ": ";}
#define result_endl() {g_outfile << std::endl;}

typedef int keytype;

#define MAX_ROOM 200
#define MAX_KEYS 400

map<keytype, int> g_keys;       // how many keys for each key types, initial configuration
keytype g_required[MAX_ROOM];   // which type of key is need for each room
vector<int> g_lockedrooms;      // how many rooms are locked.
vector<keytype> g_have[MAX_ROOM]; // key list for each room

void addkey(keytype thiskey) {
	if (g_keys.count(thiskey) > 0) {
		g_keys[thiskey]++;
	} else {
		g_keys[thiskey] = 1;
	}
}

vector<int> g_dynlockedrooms;  // dynamic rooms;
map<keytype, int> g_dynkeys;   // dynamic keys;

bool crack(int roomno, vector<int>& result) {
	if (g_dynlockedrooms.empty()) return true;	
	if (g_dynkeys.empty()) return false;
	
	keytype needkey = g_required[roomno];

	if (g_dynkeys.count(needkey) == 0 ||  //not exist
		g_dynkeys[needkey] == 0)   // existed, but not available now
		return false;

	g_dynkeys[needkey]--;
	vector<int>::iterator iter;

	// find the room and remove.
	for (iter = g_dynlockedrooms.begin(); iter != g_dynlockedrooms.end(); ++iter) {
		if (*iter == roomno) {
			break;
		}
	}
	g_dynlockedrooms.erase(iter);

	vector<keytype>::iterator keyiter;
	for (keyiter = g_have[roomno].begin(); keyiter != g_have[roomno].end(); ++keyiter) {
		if (g_dynkeys.count(*keyiter) > 0) {
			g_dynkeys[*keyiter]++;
		} else {
			g_dynkeys[*keyiter] = 1;
		}
	}

	vector<int> g_savedrooms = g_dynlockedrooms;
	map<keytype, int> g_savedkeys = g_dynkeys;	

	bool find = false;
	for(iter = g_savedrooms.begin(); iter != g_savedrooms.end(); ++iter) {
		g_dynlockedrooms = g_savedrooms;
		g_dynkeys = g_savedkeys;
		if (crack(*iter, result)) {
			find = true;
			break;
		}
	}

	if (g_savedrooms.empty())
		find = true;

	if (find) {
		result.push_back(roomno);
	}

	return find;
}

void alg() {
	int T, K, N, M, i, j;
	int thiskey;

	read(T);
	for (int caseno = 1; caseno <= T; ++caseno) {
		// clear everything for this run;
		g_keys.clear(); g_lockedrooms.clear();
		memset(g_required, 0x00, MAX_ROOM * sizeof(keytype));
		for (i = 0; i < MAX_ROOM; ++i) {
			g_have[i].clear();
		}

		read (K); read(N);

		for(i = 0; i < K; ++i) {
			read(thiskey);
			addkey(thiskey);
		}
		for (i = 0; i < N; ++i) {
			read(thiskey); read(M);
			g_required[i] = thiskey;
			for (j = 0; j < M; ++j) {
				read(thiskey);
				g_have[i].push_back(thiskey);
			}
			g_lockedrooms.push_back(i);
		}

		//crack;
		bool find = false;
		vector<int> result;
		vector<int>::iterator iter;
		for(iter = g_lockedrooms.begin(); iter != g_lockedrooms.end(); ++iter) {
			g_dynlockedrooms = g_lockedrooms;
			g_dynkeys = g_keys;

			if (crack(*iter, result)) {
				find = true;
				break;
			}
		}

		result_head(caseno);
		if (find) {
			for(i = result.size() - 1; i >=0; --i) {
				write(result[i] +  1);
				write(" ");
			}
		} else {
			write("IMPOSSIBLE");
		}

		result_endl();
	}
}


int main(int argc, _TCHAR* argv[])
{
	g_infile.open(myfile(GIN), ifstream::in);
	g_outfile.open(myfile(GOUT), ifstream::out);

	alg();

	g_outfile.close();
	g_infile.close();
	return 0;
}

#endif