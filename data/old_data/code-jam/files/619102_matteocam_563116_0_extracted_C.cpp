#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <list>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <string>
using namespace std;

int T, N, M, K;

bool bark[512][512];

int banned[512][512];

int cutted[512+1];


// matrice con all'interno la massima chess_board, inizia a -1
int m[512][512];

inline void ban(int si, int sj, int sz)
{
    int i, j;
    for (i = 0; i < sz; i++)
        for (j = 0; j < sz; j++) {
            m[si+i][sj+j] = banned[si+i][sj+j] = 1;
        }
}

inline bool check_edges(int si, int sj, int sz)
{
    int i, j;
    // check last row
    for (i = si; i < si+sz; i++)
        if (banned[i][sj+sz] || bark[i][sj+sz] == bark[i][sj+sz-1])
            return false;

    for (j = sj; j < sj+sz; j++) 
        if (banned[si+sz][j] || bark[si+sz][j] == bark[si+sz-1][j])
            return false;
    if (banned[si+sz][sj+sz])
        return false;

    return (bark[si+sz][sj+sz] != bark[si+sz][sj+sz-1] && bark[si+sz][sj+sz] != bark[si+sz-1][sj+sz]);
        
}

inline bool is_board(int si, int sj, int sz)
{
    int i, j;

    if (banned[si][sj])
        return false;

    if (m[si][sj] >= sz)
        return true;
    
    if (is_board(si, sj, sz-1) && check_edges(si, sj, sz-1)) {
        m[si][sj] = sz;
        return true;
    }
    
    
    
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        cerr << "Please, provide input file\n";
        return 1;
    }

    int i, j, t, k;
    int sz;
    ifstream in(argv[1]);

    in >> T;
    for (t = 1; t <= T; t++) {
        // code for each test case here
        int cut_types = 0;
        in >> M >> N;
        // M sono le righe
        for (i = 0; i < M; i++) {
            string s;
            char hex_c;
            for (j = 0; j < N/4; j++) {
                in >> hex_c;
                int res;
                stringstream ss;
                ss << std::hex << hex_c;
                ss >> res;
          
                for (k = 3; k >= 0; k--) {
                    bark[i][4*j+k] = res%2;
                    banned[i][4*j+k] = 0;
                    m[i][4*j+k] = 1;
                    res >>= 1;
                }
                    

            }
            
        }
        /*
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                cout << bark[i][j] << " ";
            }
            cout << endl;
        }
        */

        
        for (sz = min(N, M); sz >= 1; sz--) {
            cutted[sz] = 0;
            for (i = 0; i < M-sz+1; i++) {
                for (j = 0; j < N-sz+1; j++) {
                    m[i][j] = 1;
                    if (!banned[i][j] && is_board(i, j, sz)) {
                        //cout << "Cutting at " << i << " - " << j << " with size " << sz << endl;
                        cutted[sz]++;
                        ban(i, j, sz);
                    }
                }
            }
            
            if (cutted[sz] != 0)
                cut_types++;
        }
        
        cout << "Case #" << t << ": " << cut_types << endl;

        for (sz = min(N, M); sz >= 1; sz--) 
            if (cutted[sz] != 0)
                cout << sz << " " << cutted[sz] << endl;
    }

    
    

    return 0;
}
